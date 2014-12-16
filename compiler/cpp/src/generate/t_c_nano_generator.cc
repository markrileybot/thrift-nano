/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements. See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership. The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License. You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied. See the License for the
 * specific language governing permissions and limitations
 * under the License.
 *
 * Contains some contributions under the Thrift Software License.
 * Please see doc/old-thrift-license.txt in the Thrift distribution for
 * details.
 */

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <ctype.h>
#include <stdio.h>

#include "platform.h"
#include "t_oop_generator.h"

using std::map;
using std::ofstream;
using std::ostringstream;
using std::string;
using std::stringstream;
using std::vector;

static const string endl = "\n";  // avoid ostream << std::endl flushes


/***************************************
* UTILITY FUNCTIONS                   *
***************************************/

/**
* Upper case a string.  Wraps boost's string utility.
*/
static string tn_to_upper_case(string name) {
    string s (name);
    std::transform (s.begin(), s.end(), s.begin(), ::toupper);
    return s;
    //  return boost::to_upper_copy (name);
}

/**
* Lower case a string.  Wraps boost's string utility.
*/
static string tn_to_lower_case(string name) {
    string s (name);
    std::transform (s.begin(), s.end(), s.begin(), ::tolower);
    return s;
    //  return boost::to_lower_copy (name);
}

/**
* Makes a string friendly to C code standards by lowercasing and adding
* underscores, with the exception of the first character.  For example:
*
* Input: "ZomgCamelCase"
* Output: "zomg_camel_case"
*/
static string tn_initial_caps_to_underscores(string name) {
    string ret;
    const char *tmp = name.c_str();
    int pos = 0;

    /* the first character isn't underscored if uppercase, just lowercased */
    ret += tolower (tmp[pos]);
    pos++;
    for (unsigned int i = pos; i < name.length(); i++) {
        char lc = tolower (tmp[i]);
        if (lc != tmp[i]) {
            ret += '_';
        }
        ret += lc;
    }

    return ret;
}

static string tn_type_prefix(string name, string nspace_lc) {
    return nspace_lc + tn_initial_caps_to_underscores(name);
}

static string tn_type_name(string name, string nspace_lc) {
    return tn_type_prefix(name, nspace_lc) + "_t";
}

/* forward declarations */

/**
 * C code generator, using glib for C typing.
 */
class t_c_nano_generator : public t_oop_generator {
public:

	/* constructor */
	t_c_nano_generator(t_program *program,
			const map<string, string> &parsed_options,
			const string &option_string) : t_oop_generator(program)
{
		(void) parsed_options;
		(void) option_string;
		/* set the output directory */
		this->out_dir_base_ = "gen-c_nano";

		/* set the namespace */
		this->nspace = program_->get_namespace("c_nano");

		if (this->nspace.empty()) {
			this->nspace = "";
			this->nspace_u = "";
			this->nspace_uc = "";
			this->nspace_lc = "";
		} else {
			/* replace dots with underscores */
			char *tmp = strdup(this->nspace.c_str());
			for (unsigned int i = 0; i < strlen(tmp); i++) {
				if (tmp[i] == '.') {
					tmp[i] = '_';
				}
			}
			this->nspace = string(tmp, strlen(tmp));
			free(tmp);

			/* clean up the namespace for C.
			 * An input of 'namespace foo' should result in:
			 *  - nspace = foo       - for thrift objects and typedefs
			 *  - nspace_u = Foo     - for internal GObject prefixes
			 *  - nspace_uc = FOO_   - for macro prefixes
			 *  - nspace_lc = foo_   - for filename and method prefixes
			 * The underscores are there since uc and lc strings are used as file and
			 * variable prefixes.
			 */
			this->nspace_u = tn_initial_caps_to_underscores(this->nspace);
			this->nspace_uc = tn_to_upper_case(this->nspace_u) + "_";
			this->nspace_lc = tn_to_lower_case(this->nspace_u) + "_";
		}
}

	/* initialization and destruction */
	void init_generator();
	void close_generator();

	/* generation functions */
	void generate_typedef(t_typedef *ttypedef);
	void generate_enum(t_enum *tenum);
	void generate_consts(vector<t_const *> consts);
	void generate_struct(t_struct *tstruct);
	void generate_service(t_service *tservice);
	void generate_xception(t_struct *tstruct);

private:

	/* file streams */
	ofstream f_types_;
	ofstream f_types_impl_;
	ofstream f_header_;
	ofstream f_service_;

	/* namespace variables */
	string nspace;
	string nspace_u;
	string nspace_uc;
	string nspace_lc;

	/* helper functions */
	bool is_complex_type(t_type *ttype);
	string type_name(t_type* ttype, bool in_typedef=false, bool is_const=false);
	string base_type_name(t_base_type *type);
	string type_to_enum(t_type *type);
	string constant_value(string name, t_type *type, t_const_value *value);
	string function_signature(t_function *tfunction);
	string argument_list(t_struct *tstruct);
	string xception_list(t_struct *tstruct);
	string declare_field(t_field *tfield, bool init=false, bool pointer=false, bool constant=false, bool reference=false);
	void declare_local_variable(ofstream &out, t_type *ttype, string &base_name);

	/* generation functions */
	void generate_const_initializer(string name, t_type *type, t_const_value *value);
	void generate_service_client(t_service *tservice);
	void generate_service_server(t_service *tservice);
	void generate_object(t_struct *tstruct);
	void generate_struct_writer(ofstream &out, t_struct *tstruct, string this_name, string this_get="", bool is_function=true);
	void generate_struct_reader(ofstream &out, t_struct *tstruct, string this_name, string this_get="", bool is_function=true);
	void generate_struct_destructor(t_struct *tstruct, string type_prefix, string type_name_t);
	void generate_struct_init(t_struct *tstruct, string type_prefix, string type_name_t);
	void generate_struct_create(t_struct *tstruct, string type_prefix, string type_name_t);
	void generate_struct_reset(t_struct *tstruct, string type_prefix, string type_name_t);

	void generate_serialize_field(ofstream &out, t_field *tfield, string prefix, string suffix);
	void generate_serialize_struct(ofstream &out, t_struct *tstruct, string prefix);
	void generate_serialize_container(ofstream &out, t_type *ttype, string prefix);

	void generate_deserialize_field(ofstream &out, t_field *tfield, string prefix="", string suffix="", bool is_ptr=false, bool allocate=true);
	void generate_deserialize_struct(ofstream &out, t_struct *tstruct, string prefix, bool allocate=true);
	void generate_deserialize_container(ofstream &out, t_type *ttype, string prefix);

	void generate_comment(ofstream &out, string comment);
};

/**
 * Prepare for file generation by opening up the necessary file
 * output streams.
 */
void t_c_nano_generator::init_generator() {
	/* create output directory */
	MKDIR(get_out_dir().c_str());

	string program_name_u = tn_initial_caps_to_underscores(program_name_);
	string program_name_uc = tn_to_upper_case(program_name_u);
	string program_name_lc = tn_to_lower_case(program_name_u);

	/* create output files */
	string f_types_name = get_out_dir() + this->nspace_lc + program_name_lc + "_types.h";
	f_types_.open(f_types_name.c_str());
	string f_types_impl_name = get_out_dir() + this->nspace_lc + program_name_lc + "_types.c";
	f_types_impl_.open(f_types_impl_name.c_str());

	/* add thrift boilerplate headers */
	f_types_ << autogen_comment();
	f_types_impl_ << autogen_comment();

	/* include inclusion guard */
	f_types_ <<
			"#ifndef " << this->nspace_uc << program_name_uc << "_TYPES_H" << endl <<
			"#define " << this->nspace_uc << program_name_uc << "_TYPES_H" << endl <<
			endl;

	/* include base types */
	f_types_ << "/* base includes */" << endl << "#include <thrift-nano/thrift_nano.h>" << endl;

	/* include other thrift includes */
	const vector<t_program *> &includes = program_->get_includes();
	for (size_t i = 0; i < includes.size(); ++i) {
		f_types_ <<
				"/* other thrift includes */" << endl <<
				"#include \"" << this->nspace_lc << tn_initial_caps_to_underscores(includes[i]->get_name()) <<
				"_types.h\"" << endl;
	}
	f_types_ << endl;

	/* include custom headers */
	const vector<string> &c_includes = program_->get_c_includes();
	f_types_ << "/* custom thrift includes */" << endl;
	for (size_t i = 0; i < c_includes.size(); ++i) {
		if (c_includes[i][0] == '<') {
			f_types_ << "#include " << c_includes[i] << endl;
		} else {
			f_types_ << "#include \"" << c_includes[i] << "\"" << endl;
		}
	}
	f_types_ << endl;

	// include the types file
	f_types_impl_ << endl << "#include \"" << this->nspace_lc << program_name_u << "_types.h\"" << endl;

	// make sure we can be included in c++
	f_types_ << "#ifdef __cplusplus" << endl << "extern \"C\" {" << endl << "#endif" << endl;

	f_types_ << "/* begin types */" << endl << endl;
}

/**
 *  Finish up generation and close all file streams.
 */
void t_c_nano_generator::close_generator() {
	string program_name_uc = tn_to_upper_case
			(tn_initial_caps_to_underscores(program_name_));

	// package level init/fini
	/*
	f_types_ << "void " << this->nspace_lc << "init();" << endl;
	f_types_ << "void " << this->nspace_lc << "fini();" << endl;
	f_types_impl_ << "void " << this->nspace_lc << "init() {}" << endl;
	f_types_impl_ << "void " << this->nspace_lc << "fini() {}" << endl;
	*/

	// end cpp include
	f_types_ << "#ifdef __cplusplus" << endl << "}" << endl << "#endif" << endl;

	/* end the header inclusion guard */
	f_types_ << "#endif /* " << this->nspace_uc << program_name_uc << "_TYPES_H */" << endl;

	/* close output file */
	f_types_.close();
	f_types_impl_.close();
}

void t_c_nano_generator::generate_comment(ofstream &out, string comment) {
	generate_docstring_comment(out,
			"/**" + endl,
			" * ",
			comment,
			" */" + endl);
}

/**
 * Generates a Thrift typedef in C code.  For example:
 * 
 * Thrift: 
 * typedef map<i32,i32> SomeMap
 * 
 * C: 
 * typedef GHashTable * ThriftSomeMap;
 */
void t_c_nano_generator::generate_typedef(t_typedef* ttypedef) {
	f_types_ <<
			indent() << "typedef " << type_name(ttypedef->get_type(), true) <<
			" " << this->nspace_lc << tn_initial_caps_to_underscores(ttypedef->get_symbolic()) << "_t;"
			<< endl << endl;
} 

/**
 * Generates a C enumeration.  For example:
 *
 * Thrift:
 * enum MyEnum {
 *   ONE = 1,
 *   TWO
 * }
 *
 * C:
 * enum _ThriftMyEnum {
 *   THRIFT_MY_ENUM_ONE = 1,
 *   THRIFT_MY_ENUM_TWO
 * };
 * typedef enum _ThriftMyEnum ThriftMyEnum;
 */
void t_c_nano_generator::generate_enum(t_enum *tenum) {
	string name = tenum->get_name();
	string name_uc = tn_to_upper_case(tn_initial_caps_to_underscores(name));
	string name_lc = tn_to_lower_case(name_uc);

	f_types_ << indent() << "typedef enum " << endl << "{" << endl;

	indent_up();

	vector<t_enum_value *> constants = tenum->get_constants();
	vector<t_enum_value *>::iterator c_iter;
	bool first = true;

	/* output each of the enumeration elements */
	for (c_iter = constants.begin(); c_iter != constants.end(); ++c_iter) {
		if (first) {
			first = false;
		} else {
			f_types_ << "," << endl;
		}

		f_types_ <<
				indent() << this->nspace_uc << name_uc << "_" << (*c_iter)->get_name();
		if ((*c_iter)->has_value()) {
			f_types_ << " = " << (*c_iter)->get_value();
		}
	}

	indent_down();
	f_types_ << endl << "} " << this->nspace_lc << name_lc << "_t;" << endl << endl;
}

/**
 * Generates Thrift constants in C code.
 */
void t_c_nano_generator::generate_consts (vector<t_const *> consts) {
	f_types_ << "/* constants */" << endl;
	f_types_impl_ << "/* constants */" << endl;

	vector<t_const *>::iterator c_iter;
	for (c_iter = consts.begin(); c_iter != consts.end(); ++c_iter) {
		string name = (*c_iter)->get_name();
		string name_uc = tn_to_upper_case(name);
		string name_lc = tn_to_lower_case(name);
		t_type *type = (*c_iter)->get_type();
		t_const_value *value = (*c_iter)->get_value();

		f_types_ <<
				indent() << "#define " << this->nspace_uc << name_uc << " " <<
				constant_value (name_lc, type, value) << endl;

		generate_const_initializer (name_lc, type, value);
	}

	f_types_ << endl;
	f_types_impl_ << endl;
}

/**
 * Generate Thrift structs in C code, as GObjects.  Example:
 *
 * Thrift:
 * struct Bonk
 * {
 *   1: string message,
 *   2: i32 type
 * }
 *
 * C GObject instance header:
 * struct _ThriftBonk
 * {
 *   GObject parent;
 *
 *   gchar * message;
 *   gint32 type;
 * };
 * typedef struct _ThriftBonk ThriftBonk
 * // ... additional GObject boilerplate ...
 */
void t_c_nano_generator::generate_struct (t_struct *tstruct) {
	generate_object(tstruct);
}

/**
 * Generate C code to represent Thrift services.  Creates a new GObject
 * which can be used to access the service.
 */
void t_c_nano_generator::generate_service (t_service *tservice) {
	string svcname_u = tn_initial_caps_to_underscores(tservice->get_name());
	string svcname_uc = this->nspace_uc + tn_to_upper_case(svcname_u);
	string filename = this->nspace_lc + tn_to_lower_case(svcname_u);

	// make output files
	string f_header_name = get_out_dir() + filename + ".h";
	f_header_.open(f_header_name.c_str());

	string program_name_u = tn_initial_caps_to_underscores(program_name_);
	string program_name_lc = tn_to_lower_case(program_name_u);

	// add header file boilerplate
	f_header_ <<
			autogen_comment();

	// add an inclusion guard
	f_header_ <<
			"#ifndef " << svcname_uc << "_H" << endl <<
			"#define " << svcname_uc << "_H" << endl <<
			endl;

	// add standard includes
	f_header_ <<
			"#include \"" << this->nspace_lc << program_name_lc << "_types.h\"" << endl;

	// if we are inheriting from another service, include its header
	t_service *extends_service = tservice->get_extends();
	if (extends_service != NULL) {
		f_header_ <<
				"#include \"" << this->nspace_lc << tn_to_lower_case(tn_initial_caps_to_underscores(extends_service->get_name())) << ".h\"" << endl;
	}
	f_header_ << endl;

	// create the service implementation
	string f_service_name = get_out_dir() + filename + ".c";
	f_service_.open(f_service_name.c_str());

	// add the boilerplace header
	f_service_ <<
			autogen_comment();

	// include the headers
	f_service_ <<
			"#include <string.h>" << endl <<
			"#include <thrift/c_nano/thrift.h>" << endl <<
			"#include <thrift/c_nano/thrift_application_exception.h>" << endl <<
			"#include \"" << filename << ".h\"" << endl <<
			endl;

	// generate the client objects
	generate_service_client (tservice);

	// generate the server objects
	generate_service_server (tservice);

	// end the header inclusion guard
	f_header_ <<
			"#endif /* " << svcname_uc << "_H */" << endl;

	// close the files
	f_service_.close();
	f_header_.close();
}

/**
 *
 */
void t_c_nano_generator::generate_xception (t_struct *tstruct) {
	string name = tstruct->get_name();
	string name_u = tn_initial_caps_to_underscores(name);
	string name_lc = tn_to_lower_case(name_u);
	string name_uc = tn_to_upper_case(name_u);

	generate_object(tstruct);

	f_types_ << "/* exception */" << endl <<
			"typedef enum" << endl <<
			"{" << endl <<
			"  " << this->nspace_uc << name_uc << "_ERROR_CODE," << endl <<
			"} " << this->nspace << name << "Error;" << endl <<
			endl <<
			"GQuark " << this->nspace_lc << name_lc << "_error_quark (void);" << endl <<
			"#define " << this->nspace_uc << name_uc << "_ERROR (" <<
			this->nspace_lc << name_lc << "_error_quark())" << endl <<
			endl <<
			endl;

	f_types_impl_ <<
			"/* define the GError domain for exceptions */" << endl <<
			"#define " << this->nspace_uc << name_uc << "_ERROR_DOMAIN \"" <<
			this->nspace_lc << name_lc << "_error_quark\"" << endl <<
			"GQuark" << endl <<
			this->nspace_lc << name_lc << "_error_quark (void)" << endl <<
			"{" << endl <<
			"  return g_quark_from_static_string (" << this->nspace_uc << name_uc <<
			"_ERROR_DOMAIN);" << endl <<
			"}" << endl <<
			endl;
}

/********************
 * HELPER FUNCTIONS *
 ********************/

/**
 * Returns true if ttype is not a primitive.
 */
bool t_c_nano_generator::is_complex_type(t_type *ttype) {
	ttype = get_true_type (ttype);

	return ttype->is_container()
			|| ttype->is_struct()
			|| ttype->is_xception();
}


/**
 * Maps a Thrift t_type to a C type.
 */
string t_c_nano_generator::type_name (t_type* ttype, bool in_typedef, bool is_const) {
	(void) in_typedef;
	if (ttype->is_base_type()) {
		string bname = base_type_name ((t_base_type *) ttype);

		if (is_const) {
			return "const " + bname;
		} else {
			return bname;
		}
	}

	if (ttype->is_container()) {
		string cname;

		t_container *tcontainer = (t_container *) ttype;
		if (tcontainer->has_cpp_name()) {
			cname = tcontainer->get_cpp_name();
		} else if (ttype->is_map()) {
			cname = "tn_map_t *";
		} else if (ttype->is_set()) {
			cname = "tn_list_t *";
		} else if (ttype->is_list()) {
			cname = "tn_list_t *";
			t_type *etype = ((t_list *) ttype)->get_elem_type();
			if (etype->is_base_type()) {
				t_base_type::t_base tbase = ((t_base_type *) etype)->get_base();
				switch (tbase) {
				case t_base_type::TYPE_VOID:
					throw "compiler error: cannot determine array type";
				case t_base_type::TYPE_BOOL:
				case t_base_type::TYPE_BYTE:
				case t_base_type::TYPE_I16:
				case t_base_type::TYPE_I32:
				case t_base_type::TYPE_I64:
				case t_base_type::TYPE_DOUBLE:
				case t_base_type::TYPE_STRING:
					break;
				default:
					throw "compiler error: no array info for type";
				}
			}
		}

		if (is_const) {
			return "const " + cname;
		} else {
			return cname;
		}
	}

	// check for a namespace
    string pname = tn_type_name(ttype->get_name(), this->nspace_lc);
	if (is_complex_type (ttype)) {
		pname += " *";
	}

	if (is_const) {
		return "const " + pname;
	} else {
		return pname;
	}
}

/**
 * Maps a Thrift primitive to a C primitive.
 */
string t_c_nano_generator::base_type_name(t_base_type *type) {
	t_base_type::t_base tbase = type->get_base();

	switch (tbase) {
	case t_base_type::TYPE_VOID:
		return "void";
	case t_base_type::TYPE_STRING:
        return "tn_buffer_t *";
	case t_base_type::TYPE_BOOL:
		return "bool";
	case t_base_type::TYPE_BYTE:
		return "int8_t";
	case t_base_type::TYPE_I16:
		return "int16_t";
	case t_base_type::TYPE_I32:
		return "int32_t";
	case t_base_type::TYPE_I64:
		return "int64_t";
	case t_base_type::TYPE_DOUBLE:
		return "double";
	default:
		throw "compiler error: no C base type name for base type "
		+ t_base_type::t_base_name (tbase);
	}
}

/**
 * Returns a member of the ThriftType C enumeration in thrift_protocol.h
 * for a Thrift type.
 */
string t_c_nano_generator::type_to_enum (t_type *type) {
	type = get_true_type (type);

	if (type->is_base_type()) {
		t_base_type::t_base tbase = ((t_base_type *) type)->get_base();

		switch (tbase) {
		case t_base_type::TYPE_VOID:
			throw "NO T_VOID CONSTRUCT";
		case t_base_type::TYPE_STRING:
			return "T_STRING";
		case t_base_type::TYPE_BOOL:
			return "T_BOOL";
		case t_base_type::TYPE_BYTE:
			return "T_BYTE";
		case t_base_type::TYPE_I16:
			return "T_I16";
		case t_base_type::TYPE_I32:
			return "T_I32";
		case t_base_type::TYPE_I64:
			return "T_I64";
		case t_base_type::TYPE_DOUBLE:
			return "T_DOUBLE";
		}
	} else if (type->is_enum()) {
		return "T_I32";
	} else if (type->is_struct()) {
		return "T_STRUCT";
	} else if (type->is_xception()) {
		return "T_STRUCT";
	} else if (type->is_map()) {
		return "T_MAP";
	} else if (type->is_set()) {
		return "T_SET";
	} else if (type->is_list()) {
		return "T_LIST";
	}

	throw "INVALID TYPE IN type_to_enum: " + type->get_name();
}


/**
 * Returns C code that represents a Thrift constant.
 */
string t_c_nano_generator::constant_value(string name, t_type *type, t_const_value *value) {
	ostringstream render;

	if (type->is_base_type()) {
		/* primitives */
		t_base_type::t_base tbase = ((t_base_type *) type)->get_base();
		switch (tbase) {
		case t_base_type::TYPE_STRING:
			render << "" + value->get_string() + "\"";
			break;
		case t_base_type::TYPE_BOOL:
			render << ((value->get_integer() != 0) ? 1 : 0);
			break;
		case t_base_type::TYPE_BYTE:
		case t_base_type::TYPE_I16:
		case t_base_type::TYPE_I32:
		case t_base_type::TYPE_I64:
			render << value->get_integer();
			break;
		case t_base_type::TYPE_DOUBLE:
			if (value->get_type() == t_const_value::CV_INTEGER) {
				render << value->get_integer();
			} else {
				render << value->get_double();
			}
			break;
		default:
			throw "compiler error: no const of base type "
			+ t_base_type::t_base_name (tbase);
		}
	} else if (type->is_enum()) {
		render << "(" << type_name (type) << ")" << value->get_integer();
	} else if (type->is_struct() || type->is_xception() || type->is_list()
			|| type->is_set() || type->is_map()) {
		render << "(" << this->nspace_lc <<
				tn_to_lower_case(name) << "_constant())";
	} else {
		render << "NULL /* not supported */";
	}

	return render.str();
}

/**
 * Renders a function signature of the form 'type name(args)'
 *
 * @param tfunction Function definition
 * @return String of rendered function definition
 */
string t_c_nano_generator::function_signature(t_function* tfunction) {
	t_type* ttype = tfunction->get_returntype();
	t_struct* arglist = tfunction->get_arglist();
	t_struct* xlist = tfunction->get_xceptions();
	string fname = tn_initial_caps_to_underscores(tfunction->get_name());

	bool has_return = !ttype->is_void();
	bool has_args = arglist->get_members().size() == 0;
	bool has_xceptions = xlist->get_members().size() == 0;
	return
			"gboolean " + this->nspace_lc + fname + " (" + this->nspace
			+ service_name_ + "If * iface"
			+ (has_return ? ", " + type_name(ttype) + "* _return" : "")
			+ (has_args ? "" : (", " + argument_list (arglist)))
			+ (has_xceptions ? "" : (", " + xception_list (xlist)))
			+ ", GError ** error)";
}

/**
 * Renders a field list
 *
 * @param tstruct The struct definition
 * @return Comma sepearated list of all field names in that struct
 */
string t_c_nano_generator::argument_list (t_struct* tstruct) {
	string result = "";

	const vector<t_field*>& fields = tstruct->get_members();
	vector<t_field*>::const_iterator f_iter;
	bool first = true;
	for (f_iter = fields.begin(); f_iter != fields.end(); ++f_iter) {
		if (first) {
			first = false;
		} else {
			result += ", ";
		}
		result += type_name((*f_iter)->get_type(), false, true) + " " +
				(*f_iter)->get_name();
	}
	return result;
}

/**
 * Renders mutable exception lists
 *
 * @param tstruct The struct definition
 * @return Comma sepearated list of all field names in that struct
 */
string t_c_nano_generator::xception_list (t_struct* tstruct) {
	string result = "";

	const vector<t_field*>& fields = tstruct->get_members();
	vector<t_field*>::const_iterator f_iter;
	bool first = true;
	for (f_iter = fields.begin(); f_iter != fields.end(); ++f_iter) {
		if (first) {
			first = false;
		} else {
			result += ", ";
		}
		result += type_name((*f_iter)->get_type(), false, false) + "* " +
				(*f_iter)->get_name();
	}
	return result;
}


/**
 * Declares a field, including any necessary initialization.
 */
string t_c_nano_generator::declare_field(t_field *tfield,
		bool init,
		bool pointer,
		bool constant,
		bool reference) {
	string result = "";
	if (constant) {
		result += "const ";
	}
	result += type_name(tfield->get_type());
	if (pointer) {
		result += "*";
	}
	if (reference) {
		result += "*";
	}
	result += " " + tfield->get_name();
	if (init) {
		t_type* type = get_true_type(tfield->get_type());

		if (type->is_base_type()) {
			t_base_type::t_base tbase = ((t_base_type *) type)->get_base();
			switch (tbase) {
			case t_base_type::TYPE_VOID:
				break;
			case t_base_type::TYPE_BOOL:
			case t_base_type::TYPE_BYTE:
			case t_base_type::TYPE_I16:
			case t_base_type::TYPE_I32:
			case t_base_type::TYPE_I64:
				result += " = 0";
				break;
			case t_base_type::TYPE_DOUBLE:
				result += " = (double) 0";
				break;
			case t_base_type::TYPE_STRING:
				result += " = NULL";
				break;
			default:
				throw "compiler error: no C intializer for base type "
				+ t_base_type::t_base_name (tbase);
			}
		} else if (type->is_enum()) {
			result += " = (" + type_name (type) + ") 0";
		} else if (type->is_struct() || type->is_container()) {
			result += " = NULL";
		}
	}

	if (!reference) {
		result += ";";
	}

	return result;
}

/**
 * Generates C code that initializes complex constants.
 */
void t_c_nano_generator::generate_const_initializer(string name, t_type *type, t_const_value *value) {
}

/**
 * Generates C code that represents a Thrift service client.
 */
void t_c_nano_generator::generate_service_client(t_service *tservice) {
	/* get some C friendly service names */
	string service_name_lc = tn_to_lower_case(tn_initial_caps_to_underscores(service_name_));
	string service_name_uc = tn_to_upper_case(service_name_lc);

	// Generate the client interface dummy object in the header.
	f_header_ <<
			"/* " << service_name_ << " service interface */" << endl <<
			"typedef struct _" << this->nspace << service_name_ << "If " <<
			this->nspace << service_name_ << "If; " <<
			" /* dummy object */" << endl <<
			endl;

	// Generate the client interface object in the header.
	f_header_ <<
			"struct _" << this->nspace << service_name_ << "IfInterface" << endl <<
			"{" << endl <<
			"  GTypeInterface parent;" << endl <<
			endl;

	/* write out the functions for this interface */
	indent_up();
	vector<t_function*> functions = tservice->get_functions();
	vector<t_function*>::const_iterator f_iter;
	for (f_iter = functions.begin(); f_iter != functions.end(); ++f_iter) {
		/* make the function name C friendly */
		string funname = tn_initial_caps_to_underscores((*f_iter)->get_name());
		t_type *ttype = (*f_iter)->get_returntype();
		t_struct *arglist = (*f_iter)->get_arglist();
		t_struct *xlist = (*f_iter)->get_xceptions();
		bool has_return = !ttype->is_void();
		bool has_args = arglist->get_members().size() == 0;
		bool has_xceptions = xlist->get_members().size() == 0;

		string params = "(" + this->nspace + service_name_ + "If *iface"
				+ (has_return ? ", " + type_name (ttype) + "* _return" : "")
				+ (has_args ? "" : (", " + argument_list (arglist)))
				+ (has_xceptions ? "" : (", " + xception_list (xlist)))
				+ ", GError **error)";

		indent(f_header_) << "gboolean (*" << funname << ") " << params << ";" <<
				endl;
	}
	indent_down();

	f_header_ <<
			"};" << endl <<
			"typedef struct _" << this->nspace << service_name_ << "IfInterface " <<
			this->nspace << service_name_ << "IfInterface;" << endl <<
			endl;

	// generate all the interface boilerplate
	f_header_ <<
			"GType " << this->nspace_lc << service_name_lc <<
			"_if_get_type (void);" << endl <<
			"#define " << this->nspace_uc << "TYPE_" << service_name_uc << "_IF " <<
			"(" << this->nspace_lc << service_name_lc << "_if_get_type())" <<
			endl <<
			"#define " << this->nspace_uc << service_name_uc << "_IF(obj) " <<
			"(G_TYPE_CHECK_INSTANCE_CAST ((obj), " <<
			this->nspace_uc << "TYPE_" << service_name_uc << "_IF, " <<
			this->nspace << service_name_ << "If))" << endl <<
			"#define " << this->nspace_uc << "IS_" << service_name_uc << "_IF(obj) " <<
			"(G_TYPE_CHECK_INSTANCE_TYPE ((obj), " <<
			this->nspace_uc << "TYPE_" << service_name_uc << "_IF))" << endl <<
			"#define " << this->nspace_uc << service_name_uc <<
			"_IF_GET_INTERFACE(inst) (G_TYPE_INSTANCE_GET_INTERFACE ((inst), " <<
			this->nspace_uc << "TYPE_" << service_name_uc << "_IF, " <<
			this->nspace << service_name_ << "IfInterface))" << endl <<
			endl;

	// write out all the interface function prototypes
	for (f_iter = functions.begin(); f_iter != functions.end(); ++f_iter) {
		/* make the function name C friendly */
		string funname = tn_initial_caps_to_underscores((*f_iter)->get_name());
		t_type *ttype = (*f_iter)->get_returntype();
		t_struct *arglist = (*f_iter)->get_arglist();
		t_struct *xlist = (*f_iter)->get_xceptions();
		bool has_return = !ttype->is_void();
		bool has_args = arglist->get_members().size() == 0;
		bool has_xceptions = xlist->get_members().size() == 0;

		string params = "(" + this->nspace + service_name_ + "If *iface"
				+ (has_return ? ", " + type_name (ttype) + "* _return" : "")
				+ (has_args ? "" : (", " + argument_list (arglist)))
				+ (has_xceptions ? "" : (", " + xception_list (xlist)))
				+ ", GError **error)";

		f_header_ << "gboolean " << this->nspace_lc << service_name_lc <<
				"_if_" << funname << " " << params << ";" << endl;
	}
	f_header_ << endl;

	// Generate the client object instance definition in the header.
	f_header_ <<
			"/* " << service_name_ << " service client */" << endl <<
			"struct _" << this->nspace << service_name_ << "Client" << endl <<
			"{" << endl <<
			"  GObject parent;" << endl <<
			endl <<
			"  ThriftProtocol *input_protocol;" << endl <<
			"  ThriftProtocol *output_protocol;" << endl <<
			"};" << endl <<
			"typedef struct _" << this->nspace << service_name_ << "Client " <<
			this->nspace << service_name_ << "Client;" << endl <<
			endl;

	// Generate the class definition in the header.
	f_header_ <<
			"struct _" << this->nspace << service_name_ << "ClientClass" << endl <<
			"{" << endl <<
			"  GObjectClass parent;" << endl <<
			"};" << endl <<
			"typedef struct _" << this->nspace << service_name_ << "ClientClass " <<
			this->nspace << service_name_ << "ClientClass;" << endl <<
			endl;

	// Create all the GObject boilerplate
	f_header_ <<
			"GType " << this->nspace_lc << service_name_lc <<
			"_client_get_type (void);" << endl <<
			"#define " << this->nspace_uc << "TYPE_" << service_name_uc << "_CLIENT " <<
			"(" << this->nspace_lc << service_name_lc << "_client_get_type())" <<
			endl <<
			"#define " << this->nspace_uc << service_name_uc << "_CLIENT(obj) " <<
			"(G_TYPE_CHECK_INSTANCE_CAST ((obj), " <<
			this->nspace_uc << "TYPE_" << service_name_uc << "_CLIENT, " <<
			this->nspace << service_name_ << "Client))" << endl <<
			"#define " << this->nspace_uc << service_name_uc << "_CLIENT_CLASS(c) " <<
			"(G_TYPE_CHECK_CLASS_CAST ((c), " <<
			this->nspace_uc << "TYPE_" << service_name_uc << "_CLIENT, " <<
			this->nspace << service_name_ << "ClientClass))" << endl <<
			"#define " << this->nspace_uc << service_name_uc << "_IS_CLIENT(obj) " <<
			"(G_TYPE_CHECK_INSTANCE_TYPE ((obj), " <<
			this->nspace_uc << "TYPE_" << service_name_uc << "_CLIENT))" << endl <<
			"#define " << this->nspace_uc << service_name_uc <<
			"_IS_CLIENT_CLASS(c) " << "(G_TYPE_CHECK_CLASS_TYPE ((c), " <<
			this->nspace_uc << "TYPE_" << service_name_uc << "_CLIENT))" << endl <<
			"#define " << this->nspace_uc << service_name_uc <<
			"_CLIENT_GET_CLASS(obj) " << "(G_TYPE_INSTANCE_GET_CLASS ((obj), " <<
			this->nspace_uc << "TYPE_" << service_name_uc << "_CLIENT, " <<
			this->nspace << service_name_ << "ClientClass))" << endl <<
			endl;

	/* write out the function prototypes */
	for (f_iter = functions.begin(); f_iter != functions.end(); ++f_iter) {
		/* make the function name C friendly */
		string funname = tn_to_lower_case(tn_initial_caps_to_underscores((*f_iter)->get_name()));

		t_function service_function ((*f_iter)->get_returntype(),
				service_name_lc + string ("_client_")
				+ funname,
				(*f_iter)->get_arglist(),
				(*f_iter)->get_xceptions());
		indent(f_header_) << function_signature (&service_function) << ";" << endl;

		t_function send_function (g_type_void,
				service_name_lc + string ("_client_send_")
				+ funname,
				(*f_iter)->get_arglist());
		indent(f_header_) << function_signature (&send_function) << ";" << endl;

		// implement recv if not a oneway service
		if (!(*f_iter)->is_oneway()) {
			t_struct noargs (program_);
			t_function recv_function ((*f_iter)->get_returntype(),
					service_name_lc + string ("_client_recv_")
					+ funname,
					&noargs,
					(*f_iter)->get_xceptions());
			indent(f_header_) << function_signature (&recv_function) << ";" << endl;
		}
	}

	/* write out the get/set function prototypes */
	f_header_ << "void " + service_name_lc + "_client_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);" << endl;
	f_header_ << "void " + service_name_lc + "_client_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec);" << endl;

	f_header_ << endl;
	// end of header code

	// Generate interface method implementations
	for (f_iter = functions.begin(); f_iter != functions.end(); ++f_iter) {
		/* make the function name C friendly */
		string funname = tn_initial_caps_to_underscores((*f_iter)->get_name());
		t_type *ttype = (*f_iter)->get_returntype();
		t_struct *arglist = (*f_iter)->get_arglist();
		t_struct *xlist = (*f_iter)->get_xceptions();
		bool has_return = !ttype->is_void();
		bool has_args = arglist->get_members().size() == 0;
		bool has_xceptions = xlist->get_members().size() == 0;

		string params = "(" + this->nspace + service_name_ + "If *iface"
				+ (has_return ? ", " + type_name (ttype) + "* _return" : "")
				+ (has_args ? "" : (", " + argument_list (arglist)))
				+ (has_xceptions ? "" : (", " + xception_list (xlist)))
				+ ", GError **error)";

		string params_without_type = string("iface, ")
                                		 + (has_return ? "_return, " : "");

		const vector<t_field *>& fields = arglist->get_members();
		vector<t_field *>::const_iterator f_iter_field;
		for (f_iter_field = fields.begin(); f_iter_field != fields.end(); ++f_iter_field) {
			params_without_type += (*f_iter_field)->get_name();
			params_without_type += ", ";
		}

		const vector<t_field *>& xceptions = xlist->get_members();
		vector<t_field *>::const_iterator x_iter;
		for (x_iter = xceptions.begin(); x_iter != xceptions.end(); ++x_iter) {
			params_without_type += (*x_iter)->get_name();
			params_without_type += ", ";
		}

		f_service_ <<
				"gboolean" << endl <<
				this->nspace_lc << service_name_lc << "_if_" << funname << " " <<
				params << endl <<
				"{" << endl <<
				"  return " << this->nspace_uc << service_name_uc <<
				"_IF_GET_INTERFACE (iface)->" << funname << " (" <<
				params_without_type << "error);" << endl <<
				"}" << endl <<
				endl;
	}

	// Generate interface boilerplate
	f_service_ <<
			"GType" << endl <<
			this->nspace_lc << service_name_lc << "_if_get_type (void)" << endl <<
			"{" << endl <<
			"  static GType type = 0;" << endl <<
			"  if (type == 0)" << endl <<
			"  {" << endl <<
			"    static const GTypeInfo type_info =" << endl <<
			"    {" << endl <<
			"      sizeof (" << this->nspace << service_name_ << "IfInterface)," <<
			endl <<
			"      NULL,  /* base_init */" << endl <<
			"      NULL,  /* base_finalize */" << endl <<
			"      NULL,  /* class_init */" << endl <<
			"      NULL,  /* class_finalize */" << endl <<
			"      NULL,  /* class_data */" << endl <<
			"      0,     /* instance_size */" << endl <<
			"      0,     /* n_preallocs */" << endl <<
			"      NULL,  /* instance_init */" << endl <<
			"      NULL   /* value_table */" << endl <<
			"    };" << endl <<
			"    type = g_type_register_static (G_TYPE_INTERFACE," << endl <<
			"                                   \"" << this->nspace << service_name_ <<
			"If\"," << endl <<
			"                                   &type_info, 0);" << endl <<
			"  }" << endl <<
			"  return type;" << endl <<
			"}" << endl <<
			endl;

	// Generate client boilerplate
	f_service_ <<
			"static void " << endl <<
			this->nspace_lc << service_name_lc <<
			"_if_interface_init (" << this->nspace << service_name_ <<
			"IfInterface *iface);" << endl <<
			endl <<
			"G_DEFINE_TYPE_WITH_CODE (" << this->nspace << service_name_ <<
			"Client, " << this->nspace_lc << service_name_lc << "_client," << endl <<
			"                       G_TYPE_OBJECT, " << endl <<
			"                         G_IMPLEMENT_INTERFACE (" <<
			this->nspace_uc << "TYPE_" << service_name_uc << "_IF," << endl <<
			"                                                " <<
			this->nspace_lc << service_name_lc << "_if_interface_init));" << endl <<
			endl;

	// Generate client properties
	f_service_ <<
			"enum _" << this->nspace << service_name_ << "ClientProperties" << endl <<
			"{" << endl <<
			"  PROP_0," << endl <<
			"  PROP_" << this->nspace_uc << service_name_uc <<
			"_CLIENT_INPUT_PROTOCOL," <<
			endl <<
			"  PROP_" << this->nspace_uc << service_name_uc <<
			"_CLIENT_OUTPUT_PROTOCOL" <<
			endl <<
			"};" << endl <<
			endl;

	// generate property setter
	f_service_ <<
			"void" << endl <<
			this->nspace_lc << service_name_lc << "_client_set_property (" <<
			"GObject *object, guint property_id, const GValue *value, " <<
			"GParamSpec *pspec)" << endl <<
			"{" << endl <<
			"  " << this->nspace << service_name_ << "Client *client = " <<
			this->nspace_uc << service_name_uc << "_CLIENT (object);" << endl <<
			endl <<
			"  THRIFT_UNUSED_VAR (pspec);" << endl <<
			endl <<
			"  switch (property_id)" << endl <<
			"  {" << endl <<
			"    case PROP_" << this->nspace_uc << service_name_uc <<
			"_CLIENT_INPUT_PROTOCOL:" << endl <<
			"      client->input_protocol = g_value_get_object (value);" << endl <<
			"      break;" << endl <<
			"    case PROP_" << this->nspace_uc << service_name_uc <<
			"_CLIENT_OUTPUT_PROTOCOL:" << endl <<
			"      client->output_protocol = g_value_get_object (value);" << endl <<
			"      break;" << endl <<
			"  }" << endl <<
			"}" << endl <<
			endl;

	// generate property getter
	f_service_ <<
			"void" << endl <<
			this->nspace_lc << service_name_lc << "_client_get_property (" <<
			"GObject *object, guint property_id, GValue *value, " <<
			"GParamSpec *pspec)" << endl <<
			"{" << endl <<
			"  " << this->nspace << service_name_ << "Client *client = " <<
			this->nspace_uc << service_name_uc << "_CLIENT (object);" << endl <<
			endl <<
			"  THRIFT_UNUSED_VAR (pspec);" << endl <<
			endl <<
			"  switch (property_id)" << endl <<
			"  {" << endl <<
			"    case PROP_" << this->nspace_uc << service_name_uc <<
			"_CLIENT_INPUT_PROTOCOL:" << endl <<
			"      g_value_set_object (value, client->input_protocol);" << endl <<
			"      break;" << endl <<
			"    case PROP_" << this->nspace_uc << service_name_uc <<
			"_CLIENT_OUTPUT_PROTOCOL:" << endl <<
			"      g_value_set_object (value, client->output_protocol);" << endl <<
			"      break;" << endl <<
			"  }" << endl <<
			"}" << endl <<
			endl;


	// Generate client method implementations
	for (f_iter = functions.begin(); f_iter != functions.end(); ++f_iter) {
		string name = (*f_iter)->get_name();
		string funname = tn_initial_caps_to_underscores(name);

		// Get the struct of function call params and exceptions
		t_struct* arg_struct = (*f_iter)->get_arglist();

		// Function for sending
		t_function send_function (g_type_void,
				service_name_lc + string ("_client_send_")
				+ funname,
				(*f_iter)->get_arglist());

		// Open the send function
		indent(f_service_) <<
				function_signature (&send_function) << endl;
		scope_up(f_service_);

		// Serialize the request
		f_service_ <<
				indent() << "gint32 cseqid = 0;" << endl <<
				indent() << "ThriftProtocol * protocol = " <<
				this->nspace_uc << service_name_uc <<
				"_CLIENT (iface)->output_protocol;" << endl <<
				endl <<
				indent() << "if (protocol->tn_protocol_write_message_begin (protocol, \"" <<
				name << "\", T_CALL, cseqid, error) < 0)" << endl <<
				indent() << "  return FALSE;" << endl <<
				endl;

		generate_struct_writer (f_service_, arg_struct, "", "", false);

		f_service_ <<
				indent() <<
				"if (protocol->tn_protocol_write_message_end (protocol, error) < 0)" <<
				endl <<
				indent() <<
				"  return FALSE;" << endl <<
				indent() <<
				"if (!thrift_transport_flush (protocol->transport, error))" << endl <<
				indent() <<
				"  return FALSE;" << endl <<
				indent() <<
				"if (!thrift_transport_write_end (protocol->transport, error))" <<
				endl <<
				indent() <<
				"  return FALSE;" << endl <<
				endl <<
				indent() <<
				"return TRUE;" << endl;

		scope_down(f_service_);
		f_service_ << endl;

		// Generate recv function only if not an async function
		if (!(*f_iter)->is_oneway()) {
			t_struct noargs (program_);
			t_function recv_function ((*f_iter)->get_returntype(),
					service_name_lc
					+ string ("_client_recv_") + funname, &noargs,
					(*f_iter)->get_xceptions());
			// Open function
			indent(f_service_) <<
					function_signature (&recv_function) << endl;
			scope_up(f_service_);

			f_service_ << endl <<
					indent() << "gint32 rseqid;" << endl <<
					indent() << "gchar * fname = NULL;" << endl <<
					indent() << "ThriftMessageType mtype;" << endl <<
					indent() << "ThriftProtocol * protocol = " <<
					this->nspace_uc << service_name_uc <<
					"_CLIENT (iface)->input_protocol;" << endl <<
					endl <<
					indent() << "if (thrift_protocol_read_message_begin " <<
					"(protocol, &fname, &mtype, &rseqid, error) < 0)" <<
					endl <<
					indent() << "{" << endl <<
					indent() << "  if (fname) g_free (fname);" << endl <<
					indent() << "  return FALSE;" << endl <<
					indent() << "}" << endl <<
					endl <<
					indent() << "if (mtype == T_EXCEPTION) {" << endl <<
					indent() << "  if (fname) g_free (fname);" << endl <<
					indent() << "  ThriftApplicationException *xception = g_object_new (THRIFT_TYPE_APPLICATION_EXCEPTION, NULL);" << endl <<

					indent() << "  thrift_struct_read (THRIFT_STRUCT (xception), protocol, NULL);" << endl <<
					indent() << "  thrift_protocol_read_message_end (protocol, NULL);" << endl <<
					indent() << "  thrift_transport_read_end (protocol->transport, NULL);" << endl <<
					indent() << "  g_set_error (error, THRIFT_APPLICATION_EXCEPTION_ERROR, xception->type, \"application error: %s\", xception->message);" << endl <<
					indent() << "  g_object_unref (xception);" << endl <<
					indent() << "  return FALSE;" << endl <<
					indent() << "} else if (mtype != T_REPLY) {" << endl <<
					indent() << "  if (fname) g_free (fname);" << endl <<
					indent() << "  thrift_protocol_skip (protocol, T_STRUCT, NULL);" << endl <<
					indent() << "  thrift_protocol_read_message_end (protocol, NULL);" << endl <<
					indent() << "  thrift_transport_read_end (protocol->transport, NULL);" << endl <<
					indent() << "  g_set_error (error, THRIFT_APPLICATION_EXCEPTION_ERROR, THRIFT_APPLICATION_EXCEPTION_ERROR_INVALID_MESSAGE_TYPE, \"invalid message type %d, expected T_REPLY\", mtype);" << endl <<
					indent() << "  return FALSE;" << endl <<
					indent() << "} else if (strncmp (fname, \"" << name << "\", " << name.length() << ") != 0) {" << endl <<
					indent() << "  thrift_protocol_skip (protocol, T_STRUCT, NULL);" << endl <<
					indent() << "  thrift_protocol_read_message_end (protocol, error);" << endl <<
					indent() << "  thrift_transport_read_end (protocol->transport, error);" << endl <<
					indent() << "  g_set_error (error, THRIFT_APPLICATION_EXCEPTION_ERROR, THRIFT_APPLICATION_EXCEPTION_ERROR_WRONG_METHOD_NAME, \"wrong method name %s, expected " << name << "\", fname);" << endl <<
					indent() << "  if (fname) g_free (fname);" << endl <<
					indent() << "  return FALSE;" << endl <<
					indent() << "}" << endl <<
					indent() << "if (fname) g_free (fname);" << endl <<
					endl;

			t_struct* xs = (*f_iter)->get_xceptions();
			const std::vector<t_field*>& xceptions = xs->get_members();
			vector<t_field*>::const_iterator x_iter;

			{
				t_struct result(program_, tservice->get_name() + "_" +
						(*f_iter)->get_name() + "_result");
				t_field success((*f_iter)->get_returntype(), "*_return", 0);
				if (!(*f_iter)->get_returntype()->is_void()) {
					result.append(&success);
				}

				// add readers for exceptions, dereferencing the pointer.
				for (x_iter = xceptions.begin(); x_iter != xceptions.end(); x_iter++) {
					t_field *xception = new t_field((*x_iter)->get_type(),
							"*" + (*x_iter)->get_name(),
							(*x_iter)->get_key());
					result.append (xception);
				}

				generate_struct_reader (f_service_, &result, "", "", false);
			}

			f_service_ <<
					indent() << "if (thrift_protocol_read_message_end (protocol, error) < 0)" << endl <<
					indent() << "  return FALSE;" << endl <<
					endl <<
					indent() << "if (!thrift_transport_read_end (protocol->transport, error))" << endl <<
					indent() << "  return FALSE;" << endl <<
					endl;

			// copy over any throw exceptions and return failure
			for (x_iter = xceptions.begin(); x_iter != xceptions.end(); x_iter++) {
				f_service_ <<
						indent() << "if (*" << (*x_iter)->get_name() << " != NULL)" << endl <<
						indent() << "{" << endl <<
						indent() << "    g_set_error (error, " << this->nspace_uc <<
						tn_to_upper_case(tn_initial_caps_to_underscores(
								(*x_iter)->get_type()->get_name())) <<
								"_ERROR, " <<
								this->nspace_uc <<
								tn_to_upper_case(tn_initial_caps_to_underscores(
										(*x_iter)->get_type()->get_name())) <<
										"_ERROR_CODE, \"" << (*x_iter)->get_type()->get_name() <<
										"\");" << endl <<
										indent() << "    return FALSE;" << endl <<
										indent() << "}" << endl;
			}
			// Close function
			indent(f_service_) << "return TRUE;" << endl;
			scope_down(f_service_);
			f_service_ << endl;
		}

		// Open function
		t_function service_function((*f_iter)->get_returntype(),
				service_name_lc
				+ string ("_client_") + funname,
				(*f_iter)->get_arglist(),
				(*f_iter)->get_xceptions());
		indent(f_service_) <<
				function_signature (&service_function) << endl;
		scope_up(f_service_);

		// wrap each function
		f_service_ <<
				indent() << "if (!" << this->nspace_lc << service_name_lc <<
				"_client_send_" << funname <<
				" (iface";

		// Declare the function arguments
		const vector<t_field *> &fields = arg_struct->get_members();
		vector<t_field *>::const_iterator fld_iter;
		for (fld_iter = fields.begin(); fld_iter != fields.end(); ++fld_iter) {
			f_service_ << ", " << (*fld_iter)->get_name();
		}
		f_service_ << ", error))" << endl <<
				indent() << "  return FALSE;" << endl;

		// if not oneway, implement recv
		if (!(*f_iter)->is_oneway()) {
			string ret = (*f_iter)->get_returntype()->is_void() ? "" : "_return, ";

			const vector<t_field *>& xceptions =
					(*f_iter)->get_xceptions()->get_members();
			vector<t_field *>::const_iterator x_iter;
			for (x_iter = xceptions.begin(); x_iter != xceptions.end(); ++x_iter) {
				ret += (*x_iter)->get_name();
				ret += ", ";
			}

			f_service_ <<
					indent() << "if (!" << this->nspace_lc << service_name_lc <<
					"_client_recv_" << funname <<
					" (iface, " << ret << "error))" << endl <<
					indent() << "  return FALSE;" << endl;
		}

		// return TRUE which means all functions were called OK
		indent(f_service_) << "return TRUE;" << endl;
		scope_down(f_service_);
		f_service_ << endl;
	}

	// create the interface initializer
	f_service_ <<
			"static void" << endl <<
			this->nspace_lc << service_name_lc << "_if_interface_init (" <<
			this->nspace << service_name_ << "IfInterface *iface)" << endl <<
			"{" << endl;
	for (f_iter = functions.begin(); f_iter != functions.end(); ++f_iter) {
		/* make the function name C friendly */
		string funname = tn_initial_caps_to_underscores((*f_iter)->get_name());

		f_service_ <<
				"  iface->" << funname << " = " << this->nspace_lc << service_name_lc <<
				"_client_" << funname << ";" << endl;
	}
	f_service_ <<
			"}" << endl <<
			endl;

	// create the client instance initializer
	f_service_ <<
			"static void" << endl <<
			this->nspace_lc << service_name_lc << "_client_init (" <<
			this->nspace << service_name_ << "Client *client)" << endl <<
			"{" << endl <<
			"  client->input_protocol = NULL;" << endl <<
			"  client->output_protocol = NULL;" << endl <<
			"}" << endl <<
			endl;

	// create the client class initializer
	f_service_ <<
			"static void" << endl <<
			this->nspace_lc << service_name_lc << "_client_class_init (" <<
			this->nspace << service_name_ << "ClientClass *cls)" << endl <<
			"{" << endl <<
			"  GObjectClass *gobject_class = G_OBJECT_CLASS (cls);" << endl <<
			"  GParamSpec *param_spec;" << endl <<
			endl <<
			"  gobject_class->set_property = " << this->nspace_lc <<
			service_name_lc << "_client_set_property;" << endl <<
			"  gobject_class->get_property = " << this->nspace_lc <<
			service_name_lc << "_client_get_property;" << endl <<
			endl <<
			"  param_spec = g_param_spec_object (\"input_protocol\"," << endl <<
			"                                    \"input protocol (construct)\"," <<
			endl <<
			"                                    \"Set the client input protocol\"," <<
			endl <<
			"                                    THRIFT_TYPE_PROTOCOL," << endl <<
			"                                    G_PARAM_READWRITE);" << endl <<
			"  g_object_class_install_property (gobject_class," << endl <<
			"                                   PROP_" << this->nspace_uc <<
			service_name_uc << "_CLIENT_INPUT_PROTOCOL, param_spec);" << endl <<
			endl <<
			"  param_spec = g_param_spec_object (\"output_protocol\"," << endl <<
			"                                    \"output protocol (construct)\"," <<
			endl <<
			"                                    \"Set the client output protocol\"," <<
			endl <<
			"                                    THRIFT_TYPE_PROTOCOL," << endl <<
			"                                    G_PARAM_READWRITE);" << endl <<
			"  g_object_class_install_property (gobject_class," << endl <<
			"                                   PROP_" << this->nspace_uc <<
			service_name_uc << "_CLIENT_OUTPUT_PROTOCOL, param_spec);" << endl <<
			"}" << endl <<
			endl;
}

/**
 * Generates C code that represents a Thrift service server.
 */
void t_c_nano_generator::generate_service_server (t_service *tservice) {
	(void) tservice;
	/* get some C friendly service names */
	string service_name_u = tn_initial_caps_to_underscores(service_name_);
	string service_name_uc = tn_to_upper_case(service_name_u);

	// write the server object instance definition in the header.
	// TODO: implement after implement TServer and TProcessor
}

/**
 * Generates C code to represent a THrift structure as a GObject.
 */
void t_c_nano_generator::generate_object(t_struct *tstruct) {
	string name = tstruct->get_name();
	string name_u = tn_initial_caps_to_underscores(name);
	string name_uc = tn_to_upper_case(name_u);
	string type_prefix = this->nspace_lc + name_u;
	string type_name_t = tn_type_name(tstruct->get_name(), this->nspace_lc);

	// write the instance definition
	f_types_ << endl;
	generate_comment(f_types_, tstruct->get_doc());
	f_types_ <<
			"typedef struct " << endl <<
			"{ " << endl <<
			"  tn_struct_t parent; " << endl;

	// for each field, add a member variable
	indent_up();
	vector<t_field *>::const_iterator m_iter;
	const vector<t_field *> &members = tstruct->get_members();
	for (m_iter = members.begin(); m_iter != members.end(); ++m_iter) {
		t_type *t = get_true_type ((*m_iter)->get_type());
		if( !(*m_iter)->get_doc().empty() ) {
			generate_comment(f_types_, (*m_iter)->get_doc());
		}
		indent(f_types_) << type_name (t) << " " << (*m_iter)->get_name() << ";" << endl;
		if(!is_complex_type(t) && !t->is_string()) {
			indent(f_types_) << "bool has_" << (*m_iter)->get_name() << ";" << endl;
		}
	}
	indent_down();

	// close the structure definition and create a typedef
	f_types_ << "} " << type_name_t << ";" << endl;

	// start writing the object implementation .c file generate struct I/O methods
	string this_get = type_name_t + " *self = ("+ type_name_t + "*) data;";


	// create the destructor
	generate_struct_destructor (tstruct, type_prefix, type_name_t);

	// generate the reset
	generate_struct_reset(tstruct, type_prefix, type_name_t);

	// reader/writer
	generate_struct_reader (f_types_impl_, tstruct, "self->", this_get);
	generate_struct_writer (f_types_impl_, tstruct, "self->", this_get);

	// generate the instance init function
	generate_struct_init (tstruct, type_prefix, type_name_t);

	// generate the create function
	generate_struct_create (tstruct, type_prefix, type_name_t);
}

void t_c_nano_generator::generate_struct_create(t_struct *tstruct, string type_prefix, string type_name_t) {
	f_types_      << type_name_t << "* " << type_prefix << "_create(tn_error_t *);" << endl;
	f_types_impl_ << type_name_t << "* " << endl << type_prefix << "_create(tn_error_t *error)" << endl << "{" << endl;
	indent_up();
	indent(f_types_impl_) << type_name_t << " *object = NULL;" << endl;
	indent(f_types_impl_) << "return_if_fail(object, object = tn_alloc(sizeof(" << type_name_t << "), error));" << endl;

	vector<t_field *>::const_iterator fiter;
	const vector<t_field *> &fields = tstruct->get_members();
	for (fiter = fields.begin(); fiter != fields.end(); ++fiter) {
		t_type* t = get_true_type ((*fiter)->get_type());
		if (is_complex_type(t) || t->is_string()) {
			indent(f_types_impl_) << "object->" << (*fiter)->get_name() << " = NULL;" << endl;
		}
	}

	indent(f_types_impl_) << "return " << type_prefix << "_init(object);" << endl;
	indent_down();
	indent(f_types_impl_) << "}" << endl << endl;
}

void t_c_nano_generator::generate_struct_destructor(t_struct *tstruct, string type_prefix, string type_name_t) {
	f_types_impl_ <<
			"static void " << endl <<
			type_prefix << "_destroy(tn_object_t *object)" << endl <<
			"{" << endl;
	indent_up();

	indent(f_types_impl_) << type_name_t << " *self = (" << type_name_t << "*) object;" << endl;

	vector<t_field *>::const_iterator m_iter;
	const vector<t_field *> &members = tstruct->get_members();
	for (m_iter = members.begin(); m_iter != members.end(); ++m_iter) {
		t_type* t = get_true_type ((*m_iter)->get_type());
		string name = (*m_iter)->get_name();
		if(is_complex_type(t) || t->is_string()) {
			indent(f_types_impl_) << "if (self->" << name << " != NULL) {" << endl;
			indent_up();
			indent(f_types_impl_) << "tn_object_destroy(self->" << name << ");" << endl;
			indent(f_types_impl_) << "self->" << name << " = NULL;" << endl;
			indent_down();
			indent(f_types_impl_) << "}" << endl;
		}
	}
	indent(f_types_impl_) << "tn_free(self);" << endl;
	indent_down();
	indent(f_types_impl_) << "}" << endl << endl;
}

void t_c_nano_generator::generate_struct_reset(t_struct *tstruct, string type_prefix, string type_name_t) {
	f_types_impl_ <<
			"static void " << endl <<
			type_prefix << "_reset(tn_object_t *object)" << endl <<
			"{" << endl;
	indent_up();
	indent(f_types_impl_) << type_name_t << " *self = (" << type_name_t << "*) object;" << endl;

	vector<t_field *>::const_iterator fiter;
	const vector<t_field *> &fields = tstruct->get_members();
	for (fiter = fields.begin(); fiter != fields.end(); ++fiter) {
		t_type* t = get_true_type ((*fiter)->get_type());
		string name = (*fiter)->get_name();
		if (is_complex_type(t) || t->is_string()) {
			indent(f_types_impl_) << "if( self->"<< name <<" != NULL ) {" << endl;
			indent_up();
			indent(f_types_impl_) << "tn_object_reset(self->"<< name <<");" << endl;
			indent_down();
			indent(f_types_impl_) << "}" << endl;
		} else if (t->is_base_type()) {
			// only have init's for base types
			string dval = " = ";
			if (t->is_enum()) {
				dval += "(" + type_name (t) + ")";
			}
			t_const_value* cv = (*fiter)->get_value();
			if (cv != NULL) {
				dval += constant_value ("", t, cv);
				indent(f_types_impl_) << "self->has_" << name << " = true;" << endl;
			} else {
				dval += t->is_string() ? "NULL" : "0";
				indent(f_types_impl_) << "self->has_" << name << " = false;" << endl;
			}
			indent(f_types_impl_) << "self->" << name << dval << ";" << endl;
		}
	}

	indent_down();
	indent(f_types_impl_) << "}" << endl << endl;
}

void t_c_nano_generator::generate_struct_init(t_struct *tstruct, string type_prefix, string type_name_t) {
	f_types_ << type_name_t << "* " << type_prefix << "_init("<< type_name_t <<"*);" << endl;
	f_types_impl_ <<
			type_name_t << "* " << endl << type_prefix << "_init (" << type_name_t
			<< " *self)" << endl <<
			"{" << endl;
	indent_up();
	indent(f_types_impl_) << "self->parent.parent.tn_destroy = &" << type_prefix << "_destroy;" << endl;
	indent(f_types_impl_) << "self->parent.parent.tn_reset = &" << type_prefix << "_reset;" << endl;
	indent(f_types_impl_) << "self->parent.tn_write = &" << type_prefix << "_write;" << endl;
	indent(f_types_impl_) << "self->parent.tn_read = &" << type_prefix << "_read;" << endl;
	indent(f_types_impl_) << type_prefix << "_reset((tn_object_t*) self);" << endl;
	indent(f_types_impl_) << "return self;" << endl;
	indent_down();
	f_types_impl_ << "}" << endl << endl;
}

/**
 * Generates functions to write Thrift structures to a stream.
 */
void t_c_nano_generator::generate_struct_writer (ofstream &out,
		t_struct *tstruct,
		string this_name,
		string this_get,
		bool is_function) {
	string name = tstruct->get_name();
	string name_u = tn_initial_caps_to_underscores(name);
	string name_uc = tn_to_upper_case(name_u);
	string tn_name_t = tn_type_name(tstruct->get_name(), this->nspace_lc);

	const vector<t_field *> &fields = tstruct->get_members();
	vector <t_field *>::const_iterator f_iter;

	if (is_function) {
		indent(out) <<
				"static size_t" << endl <<
				this->nspace_lc << name_u <<
				"_write (void *data, tn_protocol_t *protocol, tn_transport_t *transport, tn_error_t *error)" << endl;
	}
	indent(out) << "{" << endl;
	indent_up();

	bool has_container = false;
	bool has_map = false;
	for (f_iter = fields.begin(); f_iter != fields.end(); ++f_iter) {
		t_type *ftype = (*f_iter)->get_type();
		if(ftype->is_container()) {
			has_container = true;
			if(ftype->is_map()) {
				has_map = true;
				break;
			}
		}
	}
	if(has_container) {
		out <<
				indent() << "tn_buffer_t buf;" << endl <<
				indent() << "size_t i = 0;" << endl <<
				indent() << "size_t size = 0;" << endl <<
				indent() << "tn_type_t value_type;" << endl;
	}
	if( has_map ) {
		out  <<
				indent() << "tn_type_t key_type;" << endl <<
				indent() << "tn_map_elem_t *e;" << endl;
	}
	out << indent() << "size_t ret = 0;" << endl << endl;
	indent(out) << this_get << endl;

	indent(out) << "return_if_fail_or_inc(ret, protocol->tn_write_struct_begin(protocol, transport, data, error));" << endl;

	for (f_iter = fields.begin(); f_iter != fields.end(); ++f_iter) {
		t_type *ftype = (*f_iter)->get_type();
		indent(out) << endl;
        if(is_complex_type(ftype) || ftype->is_string()) {
            indent(out) << "if( "<< this_name << (*f_iter)->get_name() <<" != NULL ) {" << endl;
        } else {
        	indent(out) << "if( " << this_name + "has_" + (*f_iter)->get_name()	 << " ) {" << endl;
        }
        indent_up();
		indent(out) << "return_if_fail_or_inc(ret, protocol->tn_write_field_begin (protocol, transport, " <<
				"\"" << (*f_iter)->get_name() << "\", " <<
				type_to_enum ((*f_iter)->get_type()) << ", " <<
				(*f_iter)->get_key() << ", error));" << endl;
		generate_serialize_field (out, *f_iter, this_name, "");
		indent(out) << "return_if_fail_or_inc(ret, protocol->tn_write_field_end (protocol, transport, error));" << endl;

        indent_down();
        indent(out) << "}" << endl;

	}

	// write the struct map
	indent(out) << endl;
    indent(out) << "return_if_fail_or_inc(ret, protocol->tn_write_field_stop (protocol, transport, error));" << endl;
    indent(out) << "return_if_fail_or_inc(ret, protocol->tn_write_struct_end (protocol, transport, error));" << endl;

	if (is_function) {
		indent(out) << "return ret;" << endl;
	}

	indent_down();
	indent(out) << "}" << endl << endl;
}

/**
 * Generates code to read Thrift structures from a stream.
 */
void t_c_nano_generator::generate_struct_reader(ofstream &out,
		t_struct *tstruct,
		string this_name,
		string this_get,
		bool is_function) {
	string name = tstruct->get_name();
	string name_u = tn_initial_caps_to_underscores(name);
	string name_uc = tn_to_upper_case(name_u);
	const vector<t_field *> &fields = tstruct->get_members();
	vector <t_field *>::const_iterator f_iter;

	if (is_function) {
		indent(out) <<
				"/* reads a " << name_u << " object */" << endl <<
				"static size_t" << endl << this->nspace_lc << name_u <<
				"_read (void *data, tn_protocol_t *protocol, tn_transport_t *transport, tn_error_t *error)" << endl;
	}

	indent(out) << "{" << endl;
	indent_up();

	// declare stack temp variables
	bool has_string = false;
	bool has_container = false;
	bool has_map = false;
	for (f_iter = fields.begin(); f_iter != fields.end(); ++f_iter) {
		t_type *ftype = (*f_iter)->get_type();
		if(ftype->is_string()){
			has_string = true;
		} else if(ftype->is_container()) {
			has_container = true;
			if(ftype->is_map()) {
				has_map = true;
			}
		}
	}
	if(has_string || has_container) {
		indent(out) << "int32_t size = 0;" << endl;
	}
	if(has_container) {
		out <<
				indent() << "tn_buffer_t buf;" << endl <<
				indent() << "size_t i = 0;" << endl <<
				indent() << "int32_t cont_size = 0;" << endl <<
				indent() << "tn_type_t value_type;" << endl;
	}
	if( has_map ) {
		out  <<
				indent() << "tn_type_t key_type;" << endl <<
				indent() << "tn_map_elem_t *e;" << endl;
	}
	out <<
			indent() << "size_t ret = 0;" << endl <<
			indent() << "tn_type_t ftype;" << endl <<
			indent() << "int16_t fid;" << endl <<
			indent() << this_get << endl <<
			indent() << this->nspace_lc << name_u << "_reset((tn_object_t*) self);" << endl;


	// read the beginning of the structure marker
	indent(out) << "return_if_fail_or_inc(ret, protocol->tn_read_struct_begin(protocol, transport, error));" << endl;

	// read the struct fields
	indent(out) << "while (1) {" << endl;
	indent_up();

	// read beginning field marker
	indent(out) << "return_if_fail_or_inc(ret, protocol->tn_read_field_begin(protocol, transport, NULL, &ftype, &fid, error));" << endl;

	// check for field STOP marker
	indent(out) << "if( ftype == T_STOP ) {" << endl;
	indent_up();
	indent(out) << "break;" << endl;
	indent_down();
	indent(out) << "}" << endl;

	// switch depending on the field type
	indent(out) << "switch( fid ) {" << endl;
	indent_up();

	// generate deserialization code for known types
	for (f_iter = fields.begin(); f_iter != fields.end(); ++f_iter) {
		t_type *t = get_true_type ((*f_iter)->get_type());
		
		indent(out) <<
				"case " << (*f_iter)->get_key() << ":" << endl;
		indent_up();
		indent(out) <<
				"if (ftype == " << type_to_enum ((*f_iter)->get_type()) << ") {" << endl;

        indent_up();
		// generate deserialize field

		generate_deserialize_field(out, *f_iter, this_name);

		if(!is_complex_type(t) && !t->is_string()) {
			indent(out) << "self->has_" << (*f_iter)->get_name() << " = true;" << endl;
		}

		indent_down();

		out <<
				indent() << "} else {" << endl <<
				indent() << "  return_if_fail_or_inc(ret, tn_protocol_skip(protocol, transport, ftype, error));" << endl <<
				indent() << "}" << endl <<
				indent() << "break;" << endl;
		indent_down();
	}

	// create the default case
	out <<
			indent() << "default:" << endl <<
			indent() << "  return_if_fail_or_inc(ret, tn_protocol_skip(protocol, transport, ftype, error));" << endl <<
			indent() << "  break;" << endl;

	// end switch
	indent_down();
	indent(out) << "}" << endl;

	// read field end marker
	indent(out) << "return_if_fail_or_inc(ret, protocol->tn_read_field_end(protocol, transport, error));" << endl;

	// end while loop
	indent_down();
	indent(out) << "}" << endl;

	// read the end of the structure
	indent(out) << "return_if_fail_or_inc(ret, protocol->tn_read_struct_end(protocol, transport, error));" << endl;

	if (is_function) {
		indent(out) << "return ret;" << endl;
	}

	// end the function/structure
	indent_down();
	indent(out) << "}" << endl << endl;
}

void t_c_nano_generator::generate_serialize_field(ofstream &out,
		t_field *tfield,
		string prefix,
		string suffix) {
	t_type *type = get_true_type (tfield->get_type());
	string name = prefix + tfield->get_name() + suffix;

	if (type->is_void()) {
		throw "CANNOT GENERATE SERIALIZE CODE FOR void TYPE: " + name;
	}

	if (type->is_struct() || type->is_xception()) {
		generate_serialize_struct (out, (t_struct *) type, name);
	} else if (type->is_container()) {
		generate_serialize_container (out, type, name);
	} else if (type->is_base_type() || type->is_enum()) {
		string writetype = "";
		bool is_string = false;

		if (type->is_base_type()) {
			t_base_type *tbase_type = (t_base_type *) type;
			t_base_type::t_base tbase = tbase_type->get_base();

			switch (tbase) {
			case t_base_type::TYPE_VOID:
				throw "compiler error: cannot serialize void field in a struct: "
				+ name;
				break;
			case t_base_type::TYPE_BOOL:
				writetype = "bool";
				break;
			case t_base_type::TYPE_BYTE:
				writetype = "byte";
				break;
			case t_base_type::TYPE_I16:
				writetype = "int16";
				break;
			case t_base_type::TYPE_I32:
				writetype = "int32";
				break;
			case t_base_type::TYPE_I64:
				writetype = "int64";
				break;
			case t_base_type::TYPE_DOUBLE:
				writetype = "double";
				break;
			case t_base_type::TYPE_STRING:
				is_string = true;
				if( tbase_type->is_binary() ) {
					writetype = "bytes";
				} else {
					writetype = "string";
				}
				break;
			default:
				throw "compiler error: no C writer for base type "
				+ t_base_type::t_base_name (tbase) + name;
			}

		} else if (type->is_enum()) {
			writetype = "int32";
			name = "(int32_t) " + name;
		}

		if( is_string ) {
			indent(out) << "return_if_fail_or_inc(ret, protocol->tn_write_" << writetype
					<< "_begin(protocol, transport, " << name << "->pos, error));" << endl;
		}

		indent(out) << "return_if_fail_or_inc(ret, protocol->tn_write_" << writetype
				<< "(protocol, transport, " << name << ", error));" << endl;

		if( is_string ) {
			indent(out) << "return_if_fail_or_inc(ret, protocol->tn_write_"<< writetype
					<<"_end(protocol, transport, error));" << endl;
		}
	} else {
		printf ("DO NOT KNOW HOW TO SERIALIZE FIELD '%s' TYPE '%s'\n",
				name.c_str(), type_name (type).c_str());
	}
}

void t_c_nano_generator::generate_serialize_struct(ofstream &out,
		t_struct *tstruct,
		string prefix) {
	(void) tstruct;
	indent(out) << "return_if_fail_or_inc(ret, tn_struct_write("<< prefix <<", protocol, transport, error));" << endl;
}

void t_c_nano_generator::generate_serialize_container(ofstream &out,
		t_type *ttype,
		string prefix) {
	if( ttype->is_map() ) {
		t_type *tkey = ((t_map *) ttype)->get_key_type();
		t_type *tval = ((t_map *) ttype)->get_val_type();
		string tkey_name = type_name (tkey);
		string tval_name = type_name (tval);
		string getkey = "(*("+tkey_name+"*) e->key)";
		string getval = "(*("+tval_name+"*) e->value)";

		indent(out) << "return_if_fail_or_inc(ret, protocol->tn_write_map_begin(protocol, transport, "<< prefix <<", error));" << endl;
		indent(out) << "size = " << prefix << "->kvs->elem_count;" << endl;

		// block io support
		if(!is_complex_type(tval) && !tval->is_string() && !is_complex_type(tkey) && !tkey->is_string()) {
			indent(out) << "if( protocol->block_container_io ) {" << endl;
			indent_up();
			indent(out) << "buf.buf = "<< prefix <<"->kvs->data;" << endl;
			indent(out) << "buf.len = buf.pos = size * "<< prefix <<"->kvs->elem_size;" << endl;
			indent(out) << "return_if_fail_or_inc(ret, protocol->tn_write_bytes(protocol, transport, &buf, error));" << endl;
			indent_down();
			indent(out) << "} else {" << endl;
			indent_up();
		}

		indent(out) << "for(i = 0; i < size; i++) {" << endl;
		indent_up();
		indent(out) << "e = tn_map_get(" << prefix << ",i);"<< endl;
		t_field kfield(tkey, getkey);
		generate_serialize_field (out, &kfield, "", "");
		t_field vfield(tval, getval);
		generate_serialize_field (out, &vfield, "", "");
		indent_down();
		indent(out) << "}" << endl;

		// block io support
		if(!is_complex_type(tval) && !tval->is_string() && !is_complex_type(tkey) && !tkey->is_string()) {
			indent_down();
			indent(out) << "}" << endl;
		}

		indent(out) << "return_if_fail_or_inc(ret, protocol->tn_write_map_end(protocol, transport, error));" << endl;


	} else if( ttype->is_set() ) {
		t_type *telem = ((t_set *) ttype)->get_elem_type();
		string telem_name = type_name (telem);
		string vlocal = tmp("v");

		indent(out) << "return_if_fail_or_inc(ret, protocol->tn_write_list_begin(protocol, transport, "<< prefix <<", error));" << endl;
		indent(out) << telem_name << " *" << vlocal << ";" << endl;
		indent(out) << "size = " << prefix << "->elem_count;" << endl;

		// block io support
		if(!is_complex_type(telem) && !telem->is_string()) {
			indent(out) << "if( protocol->block_container_io ) {" << endl;
			indent_up();
			indent(out) << "buf.buf = "<< prefix <<"->data;" << endl;
			indent(out) << "buf.len = buf.pos = size * "<< prefix <<"->elem_size;" << endl;
			indent(out) << "return_if_fail_or_inc(ret, protocol->tn_write_bytes(protocol, transport, &buf, error));" << endl;
			indent_down();
			indent(out) << "} else {" << endl;
			indent_up();
		}

		indent(out) << "for(i = 0; i < size; i++) {" << endl;
		indent_up();
		indent(out) << vlocal << " = tn_list_get(" << prefix << ",i);"<< endl;
		t_field vfield(telem, vlocal);
		generate_serialize_field (out, &vfield, "*", "");
		indent_down();
		indent(out) << "}" << endl;

		// block io support
		if(!is_complex_type(telem) && !telem->is_string()) {
			indent_down();
			indent(out) << "}" << endl;
		}

		indent(out) << "return_if_fail_or_inc(ret, protocol->tn_write_list_end(protocol, transport, error));" << endl;

	} else if (ttype->is_list()) {
		t_type *telem = ((t_list *) ttype)->get_elem_type();
		string telem_name = type_name (telem);
		string vlocal = tmp("v");

		indent(out) << "return_if_fail_or_inc(ret, protocol->tn_write_list_begin(protocol, transport, "<< prefix <<", error));" << endl;
		indent(out) << telem_name << " *" << vlocal << ";" << endl;
		indent(out) << "size = " << prefix << "->elem_count;" << endl;

		// block io support
		if(!is_complex_type(telem) && !telem->is_string()) {
			indent(out) << "if( protocol->block_container_io ) {" << endl;
			indent_up();
			indent(out) << "buf.buf = "<< prefix <<"->data;" << endl;
			indent(out) << "buf.len = buf.pos = size * "<< prefix <<"->elem_size;" << endl;
			indent(out) << "return_if_fail_or_inc(ret, protocol->tn_write_bytes(protocol, transport, &buf, error));" << endl;
			indent_down();
			indent(out) << "} else {" << endl;
			indent_up();
		}

		indent(out) << "for(i = 0; i < size; i++) {" << endl;
		indent_up();
		indent(out) << vlocal << " = tn_list_get(" << prefix << ",i);"<< endl;
		t_field vfield(telem, vlocal);
		generate_serialize_field (out, &vfield, "(*", ")");
		indent_down();
		indent(out) << "}" << endl;

		// block io support
		if(!is_complex_type(telem) && !telem->is_string()) {
			indent_down();
			indent(out) << "}" << endl;
		}

		indent(out) << "return_if_fail_or_inc(ret, protocol->tn_write_list_end(protocol, transport, error));" << endl;
	}
}

/* deserializes a field of any type. */
void t_c_nano_generator::generate_deserialize_field(ofstream &out,
		t_field *tfield,
		string prefix,
		string suffix,
		bool is_ptr,
		bool allocate) {
	t_type *type = get_true_type (tfield->get_type());

	if (type->is_void()) {
		throw "CANNOT GENERATE DESERIALIZE CODE FOR void TYPE: " +
		prefix + tfield->get_name();
	}

	string getptr = is_ptr ? "" : "&";
	string name = prefix + tfield->get_name() + suffix;

	if (type->is_struct() || type->is_xception()) {
		generate_deserialize_struct (out, (t_struct *) type, name, allocate);
	} else if (type->is_container()) {
		generate_deserialize_container (out, type, name);
	} else if (type->is_base_type()) {
		t_base_type *tbase_type = (t_base_type *) type;
		t_base_type::t_base tbase = tbase_type->get_base();
		if (tbase == t_base_type::TYPE_STRING) {
            if(tbase_type->is_binary()) {
                indent(out) << "return_if_fail_or_inc(ret, protocol->tn_read_bytes_begin(protocol, transport, &size, error));" << endl;
            } else {
                indent(out) << "return_if_fail_or_inc(ret, protocol->tn_read_string_begin(protocol, transport, &size, error));" << endl;
            }
            indent(out) << "if( " << name << " == NULL ) { " << endl;
            indent_up();
            indent(out) << "return_if_fail(ret, " << name << " = tn_buffer_create(size, error));" << endl;
            indent_down();
            indent(out) << "}" << endl;
		}
		indent(out) << "return_if_fail_or_inc(ret, protocol->tn_read_";

		switch (tbase) {
		case t_base_type::TYPE_VOID:
			throw "compiler error: cannot serialize void field in a struct: " + name;
			break;
		case t_base_type::TYPE_STRING:
			if (((t_base_type *) type)->is_binary()) {
				out << "bytes(protocol, transport, " << name << ", size";
			} else {
                out << "string(protocol, transport, " << name << ", size";
			}
			break;
		case t_base_type::TYPE_BOOL:
			out << "bool(protocol, transport, " << getptr << name;
			break;
		case t_base_type::TYPE_BYTE:
			out << "byte(protocol, transport, " << getptr<< name;
			break;
		case t_base_type::TYPE_I16:
			out << "int16(protocol, transport, " << getptr << name;
			break;
		case t_base_type::TYPE_I32:
			out << "int32(protocol, transport, " << getptr << name;
			break;
		case t_base_type::TYPE_I64:
			out << "int64(protocol, transport, " << getptr << name;
			break;
		case t_base_type::TYPE_DOUBLE:
			out << "double(protocol, transport, " << getptr << name;
			break;
		default:
			throw "compiler error: no C reader for base type "
			+ t_base_type::t_base_name (tbase) + name;
		}
		out << ", error));" << endl;

		// load the byte array with the data
		if (tbase == t_base_type::TYPE_STRING) {
            if(tbase_type->is_binary()) {
                indent(out) << "return_if_fail_or_inc(ret, protocol->tn_read_bytes_end(protocol, transport, error));" << endl;
            } else {
                indent(out) << "return_if_fail_or_inc(ret, protocol->tn_read_string_end(protocol, transport, error));" << endl;
            }
		}
	} else if (type->is_enum()) {
		//Need to case to int32 otherwise warning
		indent(out) << "return_if_fail_or_inc(ret, protocol->tn_read_int32(protocol, transport, (int32_t*)" << getptr << name << ", error));" << endl;
	} else {
		printf ("DO NOT KNOW HOW TO DESERIALIZE FIELD '%s' TYPE '%s'\n",
				tfield->get_name().c_str(), type_name (type).c_str());
	}
}

void t_c_nano_generator::generate_deserialize_struct(ofstream &out,
		t_struct *tstruct,
		string prefix,
		bool allocate) {
    string type_prefix = tn_type_prefix(tstruct->get_name(), this->nspace_lc);
	if (tstruct->is_xception()) {
        // TODO: handle exception
	}

    indent(out) << "if( " << prefix << " == NULL ) {" << endl;
    indent_up();
    indent(out) << "return_if_fail(ret, " << prefix << " = " << type_prefix << "_create(error));" << endl;
    indent_down();
    indent(out) << "}" << endl;
    indent(out) << "return_if_fail_or_inc(ret, tn_struct_read("<< prefix <<", protocol, transport, error));" << endl;
}

void t_c_nano_generator::generate_deserialize_container (ofstream &out, t_type *ttype,
		string prefix) {
	if (ttype->is_map()) {
        t_map  *tmap = (t_map*) ttype;
        t_type *tkey = tmap->get_key_type();
        t_type *tval = tmap->get_val_type();
		bool key_is_ptr = is_complex_type(tkey) || tkey->is_string();
		bool val_is_ptr = is_complex_type(tval) || tval->is_string();
        string kt = type_to_enum(tkey);
        string vt = type_to_enum(tval);
		string tkname = type_name(tkey);
		string tvname = type_name(tval);
		string kprefix = key_is_ptr ? "(*(" + tkname + "*)" : "((" + tkname + "*)";
		string vprefix = val_is_ptr ? "(*(" + tvname + "*)" : "((" + tvname + "*)";

		indent(out) << "return_if_fail_or_inc(ret, protocol->tn_read_map_begin(protocol, transport, &key_type, &value_type, &cont_size, error));" << endl;

        // make sure we have some kind of data
        indent(out) << "if( cont_size > 0 ) {" << endl;
        indent_up();

        // make sure the key/value types are what we expect
        indent(out) << "if( key_type == "<< kt << " && value_type == "<< vt <<" ) {" << endl;
        indent_up();

        // create and init the map
        indent(out) << "if( " << prefix << " == NULL ) {" << endl;
        indent_up();
        indent(out) << "return_if_fail(ret, "<< prefix <<" = tn_map_create(sizeof("<<
                tkname <<"), sizeof("<< tvname << "), "<< kt <<", "<< vt <<", cont_size, error));" << endl;
        indent_down();
        indent(out) << "}" << endl;
        indent(out) << "return_if_fail(ret, tn_list_ensure_cap("<< prefix <<"->kvs, cont_size, error));" << endl;

		// block io support
		if(!is_complex_type(tval) && !tval->is_string() && !is_complex_type(tkey) && !tkey->is_string()) {
			indent(out) << "if( protocol->block_container_io ) {" << endl;
			indent_up();
			indent(out) << "buf.pos = 0;" << endl;
			indent(out) << "buf.buf = "<< prefix <<"->kvs->data;" << endl;
			indent(out) << "buf.len = cont_size * "<< prefix <<"->kvs->elem_size;" << endl;
			indent(out) << "return_if_fail_or_inc(ret, protocol->tn_read_bytes(protocol, transport, &buf, buf.len, error));" << endl;
			indent(out) << prefix << "->kvs->elem_count = cont_size;" << endl;
			indent(out) << "return_if_fail(ret, tn_map_rebuild("<< prefix <<", error));" << endl;
			indent_down();
			indent(out) << "} else {" << endl;
			indent_up();
		}

        // read the data
		indent(out) << "do {" << endl;
        indent_up();
		indent(out) << "return_if_fail(ret, e = tn_map_append("<< prefix <<", error));" << endl;
        t_field fkey (tkey, "e->key");
        generate_deserialize_field (out, &fkey, kprefix, ")", true);
        t_field fval (tval, "e->value");
        generate_deserialize_field (out, &fval, vprefix, ")", true);
        indent(out) << "tn_map_put("<< prefix <<", e);" << endl;
        indent_down();
        indent(out) << "} while(--cont_size);" << endl;

		// block io support
		if(!is_complex_type(tval) && !tval->is_string() && !is_complex_type(tkey) && !tkey->is_string()) {
			indent_down();
			indent(out) << "}" << endl;
		}

        // end check types
        indent_down();
        indent(out) << "} else {" << endl;
        indent_up();
        indent(out) << "do {" << endl;
        indent_up();
        indent(out) << "return_if_fail_or_inc(ret, tn_protocol_skip(protocol, transport, key_type, error));" << endl;
        indent(out) << "return_if_fail_or_inc(ret, tn_protocol_skip(protocol, transport, value_type, error));" << endl;
        indent_down();
        indent(out) << "} while(--cont_size);" << endl;
        indent_down();
        indent(out) << "}" << endl;

        // end check size
        indent_down();
        indent(out) << "}" << endl;

		// read map end
		indent(out) << "return_if_fail_or_inc(ret, protocol->tn_read_map_end(protocol, transport, error));" << endl;
	} else if (ttype->is_set()) {
        t_set  *tset = (t_set*) ttype;
        t_type *tval = tset->get_elem_type();
        string vt = type_to_enum(tval);
		string tname = type_name(tval);
		string ptrval = (tval->is_string() || is_complex_type(tval)) ? "*" : "";

        indent(out) << "return_if_fail_or_inc(ret, protocol->tn_read_list_begin(protocol, transport, &value_type, &cont_size, error));" << endl;

        // make sure we have some kind of data
        indent(out) << "if( cont_size > 0 ) {" << endl;
        indent_up();

        // make sure the key/value types are what we expect
        indent(out) << "if( value_type == "<< vt <<" ) {" << endl;
        indent_up();

        // declare locals
        string vlocal = tmp("v");
		string ptrvlocal = "*" + vlocal;
        declare_local_variable(out, tval, ptrvlocal);

        // create and init the list
        indent(out) << "if( "<< prefix <<" == NULL ) {" << endl;
        indent_up();
        indent(out) << "return_if_fail(ret, "<< prefix <<" = tn_list_create(sizeof("<< tname <<"), cont_size, "<< vt <<", error));" << endl;
        indent(out) << "return_if_fail(ret, tn_list_ensure_cap("<< prefix <<", cont_size, error));" << endl;
        indent_down();
        indent(out) << "}" << endl;

		// block io support
		if(!is_complex_type(tval) && !tval->is_string()) {
			indent(out) << "if( protocol->block_container_io ) {" << endl;
			indent_up();
			indent(out) << "buf.pos = 0;" << endl;
			indent(out) << "buf.buf = "<< prefix <<"->data;" << endl;
			indent(out) << "buf.len = cont_size * "<< prefix <<"->elem_size;" << endl;
			indent(out) << "return_if_fail_or_inc(ret, protocol->tn_read_bytes(protocol, transport, &buf, buf.len, error));" << endl;
			indent(out) << prefix << "->elem_count = cont_size;" << endl;
			indent_down();
			indent(out) << "} else {" << endl;
			indent_up();
		}

		// iterate over the elements
		indent(out) << "do {" << endl;
        indent_up();
        indent(out) << "return_if_fail(ret, "<< vlocal <<" = tn_list_append("<< prefix <<", error));" << endl;
        t_field fval (tval, vlocal);
        generate_deserialize_field(out, &fval, ptrval, "", true);
        indent_down();
        indent(out) << "} while(--cont_size);" << endl;

		// block io support
		if(!is_complex_type(tval) && !tval->is_string()) {
			indent_down();
			indent(out) << "}" << endl;
		}

        // end check type
        indent_down();
        indent(out) << "} else {" << endl;
        indent_up();
        indent(out) << "do {" << endl;
        indent_up();
        indent(out) << "return_if_fail_or_inc(ret, tn_protocol_skip(protocol, transport, value_type, error));" << endl;
        indent_down();
		indent(out) << "} while(--cont_size);" << endl;
        indent_down();
        indent(out) << "}" << endl;

        // end check size
        indent_down();
        indent(out) << "}" << endl;

		// read set end
        indent(out) << "return_if_fail_or_inc(ret, protocol->tn_read_list_end(protocol, transport, error));" << endl;
	} else if (ttype->is_list()) {
        t_list *tlist = (t_list*) ttype;
        t_type *tval  = tlist->get_elem_type();
        string vt = type_to_enum(tval);
		string tname = type_name(tval);
		string ptrval = (tval->is_string() || is_complex_type(tval)) ? "*" : "";

        indent(out) << "return_if_fail_or_inc(ret, protocol->tn_read_list_begin(protocol, transport, &value_type, &cont_size, error));" << endl;

        // make sure we have some kind of data
        indent(out) << "if( cont_size > 0 ) {" << endl;
        indent_up();

        // make sure the key/value types are what we expect
        indent(out) << "if( value_type == "<< vt <<" ) {" << endl;
        indent_up();

        // declare locals
        string vlocal = tmp("v");
		string ptrvlocal = "*" + vlocal;
		declare_local_variable(out, tval, ptrvlocal);

        // create and init the list
        indent(out) << "if( "<< prefix <<" == NULL ) {" << endl;
        indent_up();
        indent(out) << "return_if_fail(ret, "<< prefix <<" = tn_list_create(sizeof("<< tname <<"), cont_size, "<< vt <<", error));" << endl;
        indent(out) << "return_if_fail(ret, tn_list_ensure_cap("<< prefix <<", cont_size, error));" << endl;
        indent_down();
        indent(out) << "}" << endl;

		// block io support
		if(!is_complex_type(tval) && !tval->is_string()) {
			indent(out) << "if( protocol->block_container_io ) {" << endl;
			indent_up();
			indent(out) << "buf.pos = 0;" << endl;
			indent(out) << "buf.buf = "<< prefix <<"->data;" << endl;
			indent(out) << "buf.len = cont_size * "<< prefix <<"->elem_size;" << endl;
			indent(out) << "return_if_fail_or_inc(ret, protocol->tn_read_bytes(protocol, transport, &buf, buf.len, error));" << endl;
			indent(out) << prefix << "->elem_count = cont_size;" << endl;
			indent_down();
			indent(out) << "} else {" << endl;
			indent_up();
		}

        // iterate over the elements
        indent(out) << "do {" << endl;
        indent_up();
        indent(out) << "return_if_fail(ret, "<< vlocal <<" = tn_list_append("<< prefix <<", error));" << endl;
        t_field fval (tval, vlocal);
        generate_deserialize_field(out, &fval, ptrval, "", true);
        indent_down();
		indent(out) << "} while(--cont_size);" << endl;

		// block io support
		if(!is_complex_type(tval) && !tval->is_string()) {
			indent_down();
			indent(out) << "}" << endl;
		}

        // end check type
        indent_down();
        indent(out) << "} else {" << endl;
        indent_up();
        indent(out) << "do {" << endl;
        indent_up();
        indent(out) << "return_if_fail_or_inc(ret, tn_protocol_skip(protocol, transport, value_type, error));" << endl;
        indent_down();
		indent(out) << "} while(--cont_size);" << endl;
        indent_down();
        indent(out) << "}" << endl;

        // end check size
        indent_down();
        indent(out) << "}" << endl;

        // read set end
        indent(out) << "return_if_fail_or_inc(ret, protocol->tn_read_list_end(protocol, transport, error));" << endl;
	}
}

void t_c_nano_generator::declare_local_variable(ofstream &out, t_type *ttype, string &name) {
	string tname = type_name (ttype);
	string ptr = ttype->is_base_type() && !ttype->is_string() ? " " : " *";
//    indent(out) << tname << ptr << name << ";" << endl;
    indent(out) << tname << " " << name << ";" << endl;
}


/* register this generator with the main program */
THRIFT_REGISTER_GENERATOR(c_nano, "C, using thrift nano lib", "")

