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
 */

#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <map>

#include <stdlib.h>
#include <sys/stat.h>
#include <sstream>
#include "t_generator.h"
#include "platform.h"

using std::map;
using std::ofstream;
using std::ostringstream;
using std::pair;
using std::string;
using std::stringstream;
using std::vector;

static const string endl = "\n";  // avoid ostream << std::endl flushes
static const string HDR_PROGRAM = "# ";
static const string HDR_SECTION = "## ";
static const string HDR_ENUM = "### ";
static const string HDR_CONST = "### ";
static const string HDR_TYPDEF = "### ";
static const string HDR_STRUCT = "### ";
static const string HDR_SERVICE = "### ";
static const string HDR_FUNCTION = "#### ";
static const string HDR_ARGS = "##### ";
static const string HDR_DEF = "##### ";


/* forward declarations */
static string ltrim(string name);
static string rtrim(string name);
static string trim(string name);

enum input_type { INPUT_UNKNOWN, INPUT_UTF8, INPUT_PLAIN };

/**
 * HTML code generator
 *
 * mostly copy/pasting/tweaking from mcslee's work.
 */
class t_md_generator : public t_generator {
 public:
  t_md_generator(
      t_program* program,
      const std::map<std::string, std::string>& parsed_options,
      const std::string& option_string)
    : t_generator(program)
  {
    (void) parsed_options;
    (void) option_string;  
    out_dir_base_ = "gen-md";
    input_type_ = INPUT_UNKNOWN;
    
    escape_.clear();
    escape_['&']  = "&amp;";
    escape_['<']  = "&lt;";
    escape_['>']  = "&gt;";
    escape_['"']  = "&quot;";
    escape_['\''] = "&apos;";

    init_allowed__markup();
  }

  void generate_program();
  void generate_program_toc();
  void generate_program_toc_row(t_program* tprog);
  void generate_program_toc_rows(t_program* tprog,
         std::vector<t_program*>& finished);
  void generate_index();
  std::string escape_html(std::string const & str);
  std::string escape_html_tags(std::string const & str);
  std::string make_file_link( std::string name);
  bool is_utf8_sequence(std::string const & str, size_t firstpos);
  void detect_input_encoding(std::string const & str, size_t firstpos);
  void init_allowed__markup();
  
  /**
   * Program-level generation functions
   */

  void generate_typedef (t_typedef*  ttypedef);
  void generate_enum    (t_enum*     tenum);
  void generate_const   (t_const*    tconst);
  void generate_struct  (t_struct*   tstruct);
  void generate_service (t_service*  tservice);
  void generate_xception(t_struct*   txception);

  void print_doc        (t_doc* tdoc);
  int  print_type       (t_type* ttype);
  void print_const_value(t_type* type, t_const_value* tvalue);
  void print_fn_args_doc(t_function* tfunction);

 private:
  string to_lower_case(string name);
  std::ofstream f_out_;
  std::string  current_file_;
  input_type input_type_;
  std::map<std::string, int> allowed_markup; 
};


/**
 * Emits the Table of Contents links at the top of the module's page
 */
void t_md_generator::generate_program_toc() {
  generate_program_toc_row(program_);
  f_out_ << endl;
}


/**
 * Recurses through from the provided program and generates a ToC row
 * for each discovered program exactly once by maintaining the list of
 * completed rows in 'finished'
 */
void t_md_generator::generate_program_toc_rows(t_program* tprog,
         std::vector<t_program*>& finished) {
  for (vector<t_program*>::iterator iter = finished.begin();
       iter != finished.end(); iter++) {
    if (tprog->get_path() == (*iter)->get_path()) {
      return;
    }
  }
  finished.push_back(tprog);
  generate_program_toc_row(tprog);
  vector<t_program*> includes = tprog->get_includes();
  for (vector<t_program*>::iterator iter = includes.begin();
       iter != includes.end(); iter++) {
    generate_program_toc_rows(*iter, finished);
  }
}

/**
 * Emits the Table of Contents links at the top of the module's page
 */
void t_md_generator::generate_program_toc_row(t_program* tprog) {
  string fname = tprog->get_name() + ".md";
  f_out_ << HDR_PROGRAM << tprog->get_name() << endl << endl;
  f_out_ << "([Home](index.md))" << endl;
  if (!tprog->get_services().empty()) {
	f_out_ << "* " << "[Services](" << make_file_link(fname) << "#services)" << endl;
    vector<t_service*> services = tprog->get_services();
    vector<t_service*>::iterator sv_iter;
    for (sv_iter = services.begin(); sv_iter != services.end(); ++sv_iter) {
      string name = get_service_name(*sv_iter);
      f_out_ << "  * [" << name << "]" << "(" << make_file_link(fname) << "#" << to_lower_case(name) << ")" << endl;
      map<string,string> fn_html;
      vector<t_function*> functions = (*sv_iter)->get_functions();
      vector<t_function*>::iterator fn_iter;
      for (fn_iter = functions.begin(); fn_iter != functions.end(); ++fn_iter) {
        string fn_name = (*fn_iter)->get_name();
        f_out_ <<  "    * [" + fn_name + "](" + make_file_link(fname) + "#" + to_lower_case(name) + ")" << endl;
      }
    }
  }
  f_out_ << endl << endl;
  if (!tprog->get_enums().empty()) {
	f_out_ << "* " << "[Enumerations](" << make_file_link(fname) << "#enumerations)" << endl;
    vector<t_enum*> enums = tprog->get_enums();
    vector<t_enum*>::iterator en_iter;
    for (en_iter = enums.begin(); en_iter != enums.end(); ++en_iter) {
      string name = (*en_iter)->get_name();
      f_out_ << "  * [" +  name + "](" + make_file_link(fname) + "#" + to_lower_case(name) + ")" << endl;
    }
  }
  if (!tprog->get_typedefs().empty()) {
	f_out_ << "* " << "[Type declarations](" << make_file_link(fname) << "#type-declarations)" << endl;
    vector<t_typedef*> typedefs = tprog->get_typedefs();
    vector<t_typedef*>::iterator td_iter;
    for (td_iter = typedefs.begin(); td_iter != typedefs.end(); ++td_iter) {
      string name = (*td_iter)->get_symbolic();
      f_out_ << "  * [" +  name + "](" + make_file_link(fname) + "#" + to_lower_case(name) + ")" << endl;
    }
  }
  if (!tprog->get_objects().empty()) {
	f_out_ << "* " << "[Data structures](" << make_file_link(fname) << "#data-structures)" << endl;
	vector<t_struct*> objects = tprog->get_objects();
    vector<t_struct*>::iterator o_iter;
    for (o_iter = objects.begin(); o_iter != objects.end(); ++o_iter) {
      string name = (*o_iter)->get_name();
      f_out_ << "  * [" +  name + "](" + make_file_link(fname) + "#" + to_lower_case(name) + ")" << endl;
    }
  }
  if (!tprog->get_consts().empty()) {
	f_out_ << "* " << "[Constants](" << make_file_link(fname) << "#constants)" << endl;
    vector<t_const*> consts = tprog->get_consts();
    vector<t_const*>::iterator con_iter;
    for (con_iter = consts.begin(); con_iter != consts.end(); ++con_iter) {
      string name = (*con_iter)->get_name();
      f_out_ << "  * [" +  name + "](" + make_file_link(fname) + "#" + to_lower_case(name) + ")" << endl;
    }
  }
  f_out_ << endl << endl;
}

/**
 * Prepares for file generation by opening up the necessary file output
 * stream.
 */
void t_md_generator::generate_program() {
  // Make output directory
  MKDIR(get_out_dir().c_str());
  current_file_ =  program_->get_name() + ".md";
  string fname = get_out_dir() + current_file_;
  f_out_.open(fname.c_str());

  print_doc(program_);

  generate_program_toc();

  if (!program_->get_consts().empty()) {
    f_out_ << HDR_SECTION << "Constants" << endl;
    vector<t_const*> consts = program_->get_consts();
    generate_consts(consts);
  }

  if (!program_->get_enums().empty()) {
	  f_out_ << HDR_SECTION << "Enumerations" << endl;
    // Generate enums
    vector<t_enum*> enums = program_->get_enums();
    vector<t_enum*>::iterator en_iter;
    for (en_iter = enums.begin(); en_iter != enums.end(); ++en_iter) {
      generate_enum(*en_iter);
    }
  }

  if (!program_->get_typedefs().empty()) {
    f_out_ << HDR_SECTION << "Type declarations" << endl;
    // Generate typedefs
    vector<t_typedef*> typedefs = program_->get_typedefs();
    vector<t_typedef*>::iterator td_iter;
    for (td_iter = typedefs.begin(); td_iter != typedefs.end(); ++td_iter) {
      generate_typedef(*td_iter);
    }
  }

  if (!program_->get_objects().empty()) {
    f_out_ << HDR_SECTION << "Data structures" << endl;
    // Generate structs and exceptions in declared order
    vector<t_struct*> objects = program_->get_objects();
    vector<t_struct*>::iterator o_iter;
    for (o_iter = objects.begin(); o_iter != objects.end(); ++o_iter) {
      if ((*o_iter)->is_xception()) {
        generate_xception(*o_iter);
      } else {
        generate_struct(*o_iter);
      }
    }
  }

  if (!program_->get_services().empty()) {
    f_out_ << HDR_SECTION << "Services" << endl;
    // Generate services
    vector<t_service*> services = program_->get_services();
    vector<t_service*>::iterator sv_iter;
    for (sv_iter = services.begin(); sv_iter != services.end(); ++sv_iter) {
      service_name_ = get_service_name(*sv_iter);
      generate_service(*sv_iter);
    }
  }

  f_out_.close();
  
  generate_index();
}


/**
 * Emits the index.html file for the recursive set of Thrift programs
 */
void t_md_generator::generate_index() {
  current_file_ = "index.md";
  string index_fname = get_out_dir() + current_file_;
  f_out_.open(index_fname.c_str());
  f_out_ << HDR_PROGRAM << "All Thrift declarations" << endl;
  vector<t_program*> programs;
  generate_program_toc_rows(program_, programs);
  f_out_.close();
}

/**
 * Returns the target file for a <a href> link 
 * The returned string is empty, whenever filename refers to the current file.
 */
std::string t_md_generator::make_file_link( std::string filename) {
  return (current_file_.compare(filename) != 0)  ?  filename  :  "";
}

/**
 * If the provided documentable object has documentation attached, this
 * will emit it to the output stream in HTML format.
 */
void t_md_generator::print_doc(t_doc* tdoc) {
  if (tdoc->has_doc()) {
    f_out_ << trim(tdoc->get_doc()) << endl;
  }
}

bool t_md_generator::is_utf8_sequence(std::string const & str, size_t firstpos) {
  // leading char determines the length of the sequence
  unsigned char c = str.at(firstpos);
  int count = 0;
  if(        (c & 0xE0) == 0xC0) {
    count = 1;
  } else if( (c & 0xF0) == 0xE0) {
    count = 2;
  } else if( (c & 0xF8) == 0xF0) {
    count = 3;
  } else if( (c & 0xFC) == 0xF8) {
    count = 4;
  } else if( (c & 0xFE) == 0xFC) {
    count = 5;
  } else {
    //pdebug("UTF-8 test: char '%c' (%d) is not a valid UTF-8 leading byte", c, int(c));
    return false;  // no UTF-8
  }

  // following chars
  size_t pos = firstpos + 1;
  while( (pos < str.length()) && (0 < count))
  {
    c = str.at(pos);
    if( (c & 0xC0) !=  0x80) {
      //pdebug("UTF-8 test: char '%c' (%d) is not a valid UTF-8 following byte", c, int(c));
      return false;  // no UTF-8
    }    
    --count;
    ++pos;
  }
  
  // true if the sequence is complete
  return (0 == count);
}

void t_md_generator::detect_input_encoding(std::string const & str, size_t firstpos) {
  if( is_utf8_sequence(str,firstpos))
  {
    pdebug( "Input seems to be already UTF-8 encoded");
    input_type_ = INPUT_UTF8;
    return;
  }

  // fallback 
  pwarning( 1, "Input is not UTF-8, treating as plain ANSI");
  input_type_ = INPUT_PLAIN;
}

void t_md_generator::init_allowed__markup() {
  allowed_markup.clear();
  // standalone tags
  allowed_markup["br"] = 1;
  allowed_markup["br/"] = 1;
  allowed_markup["img"] = 1;
  // paired tags
  allowed_markup["b"] = 1;
  allowed_markup["/b"] = 1;
  allowed_markup["u"] = 1;
  allowed_markup["/u"] = 1;
  allowed_markup["i"] = 1;
  allowed_markup["/i"] = 1;
  allowed_markup["s"] = 1;
  allowed_markup["/s"] = 1;
  allowed_markup["big"] = 1;
  allowed_markup["/big"] = 1;
  allowed_markup["small"] = 1;
  allowed_markup["/small"] = 1;
  allowed_markup["sup"] = 1;
  allowed_markup["/sup"] = 1;
  allowed_markup["sub"] = 1;
  allowed_markup["/sub"] = 1;
  allowed_markup["pre"] = 1;
  allowed_markup["/pre"] = 1;
  allowed_markup["tt"] = 1;
  allowed_markup["/tt"] = 1;
  allowed_markup["ul"] = 1;
  allowed_markup["/ul"] = 1;
  allowed_markup["ol"] = 1;
  allowed_markup["/ol"] = 1;
  allowed_markup["li"] = 1;
  allowed_markup["/li"] = 1;
  allowed_markup["a"] = 1;
  allowed_markup["/a"] = 1;
  allowed_markup["p"] = 1;
  allowed_markup["/p"] = 1;
  allowed_markup["code"] = 1;
  allowed_markup["/code"] = 1;
  allowed_markup["dl"] = 1;
  allowed_markup["/dl"] = 1;
  allowed_markup["dt"] = 1;
  allowed_markup["/dt"] = 1;
  allowed_markup["dd"] = 1;
  allowed_markup["/dd"] = 1;
  allowed_markup["h1"] = 1;
  allowed_markup["/h1"] = 1;
  allowed_markup["h2"] = 1;
  allowed_markup["/h2"] = 1;
  allowed_markup["h3"] = 1;
  allowed_markup["/h3"] = 1;
  allowed_markup["h4"] = 1;
  allowed_markup["/h4"] = 1;
  allowed_markup["h5"] = 1;
  allowed_markup["/h5"] = 1;
  allowed_markup["h6"] = 1;
  allowed_markup["/h6"] = 1;
}
  
std::string t_md_generator::escape_html_tags(std::string const & str) {
  std::ostringstream result;

  unsigned char c = '?';
  size_t lastpos;
  size_t firstpos = 0;
  while( firstpos < str.length()) {

    // look for non-ASCII char  
    lastpos = firstpos;    
    while( lastpos < str.length()) {
      c = str.at(lastpos);
      if( ('<' == c) || ('>' == c)) {
        break;
      }
      ++lastpos;
    }
    
    // copy what we got so far    
    if( lastpos > firstpos) {
      result << str.substr( firstpos, lastpos-firstpos);
      firstpos = lastpos;
    }
    
    // reached the end?
    if( firstpos >= str.length()) {
      break;
    }

    // tag end without corresponding begin
    ++firstpos;
    if( '>' == c) {
      result << "&gt;";
      continue;
    }

    // extract the tag
    std::ostringstream tagstream;
    while( firstpos < str.length()) {
      c = str.at(firstpos);
      ++firstpos;
      if('<'==c) {
        tagstream << "&lt;"; // nested begin?
      } else if('>'==c) {
        break;
      } else {
        tagstream << c;  // not very efficient, but tags should be quite short
      }
    }

    // we allow for several markup in docstrings, all else will become escaped
    string tag_content = tagstream.str();
    string tag_key = tag_content;
    size_t first_white = tag_key.find_first_of(" \t\f\v\n\r");
    if( first_white != string::npos) {
      tag_key.erase(first_white);
    }
    for (std::string::size_type i=0; i<tag_key.length(); ++i) {
      tag_key[i] = tolower(tag_key[i]);
    }
    if( allowed_markup.find(tag_key) != allowed_markup.end()) {
      result << "<" << tag_content << ">"; 
    } else {
      result << "&lt;" << tagstream.str() << "&gt;"; 
      pverbose("illegal markup <%s> in doc-comment\n", tag_key.c_str());
    }
  }
  
  return result.str();
}

std::string t_md_generator::escape_html(std::string const & str) {
  // the generated HTML header says it is UTF-8 encoded
  // if UTF-8 input has been detected before, we don't need to change anything
  if( input_type_ == INPUT_UTF8) {
    return escape_html_tags(str);
  }
  
  // convert unsafe chars to their &#<num>; equivalent
  std::ostringstream result;
  unsigned char c = '?';
  unsigned int ic = 0;
  size_t lastpos;
  size_t firstpos = 0;
  while( firstpos < str.length()) {

    // look for non-ASCII char  
    lastpos = firstpos;    
    while( lastpos < str.length()) {
      c = str.at(lastpos);
      ic = c;
      if( (32 > ic) || (127 < ic)) {
        break;
      }
      ++lastpos;
    }
    
    // copy what we got so far    
    if( lastpos > firstpos) {
      result << str.substr( firstpos, lastpos-firstpos);
      firstpos = lastpos;
    }

    // reached the end?
    if( firstpos >= str.length()) {
      break;
    }

    // some control code?
    if( (0 <= ic) && (31 >= ic))
    {
      switch( c)
      {
        case '\r' :  
        case '\n' :  
        case '\t' :
          result << c;
          break;
        default: // silently consume all other ctrl chars
          break;
      }
      ++firstpos;
      continue;        
    }
    
    // reached the end?
    if( firstpos >= str.length()) {
      break;
    }

    // try to detect input encoding
    if( input_type_ == INPUT_UNKNOWN) {
      detect_input_encoding(str,firstpos);
      if( input_type_ == INPUT_UTF8) {
        lastpos = str.length();
        result << str.substr( firstpos, lastpos-firstpos);
        break;
      }
    }
    
    // convert the character to something useful based on the detected encoding
    switch( input_type_) {
      case INPUT_PLAIN: 
        result << "&#" << ic << ";";  
        ++firstpos;
        break;
      default:
        throw "Unexpected or unrecognized input encoding";
    }
  }
  
  return escape_html_tags(result.str());
}

/**
 * Prints out the provided type in HTML
 */
int t_md_generator::print_type(t_type* ttype) {
  int len = 0;
  if (ttype->is_container()) {
    if (ttype->is_list()) {
      f_out_ << "list&lt;";
      len = 6 + print_type(((t_list*)ttype)->get_elem_type());
      f_out_ << "&gt;";
    } else if (ttype->is_set()) {
      f_out_ << "set&lt;";
      len = 5 + print_type(((t_set*)ttype)->get_elem_type());
      f_out_ << "&gt;";
    } else if (ttype->is_map()) {
      f_out_ << "map&lt;";
      len = 5 + print_type(((t_map*)ttype)->get_key_type());
      f_out_ << ", ";
      len += print_type(((t_map*)ttype)->get_val_type());
      f_out_ << "&gt;";
    }
  } else if (ttype->is_base_type()) {
    f_out_ << (((t_base_type*)ttype)->is_binary() ? "binary" : ttype->get_name());
    len = ttype->get_name().size();
  } else {
    string prog_name = ttype->get_program()->get_name();
    string type_name = ttype->get_name();
    f_out_ << "[" << type_name << "](#" << to_lower_case(type_name) << ")";
    len = type_name.size();
  }
  return len;
}

/**
 * Prints out an HTML representation of the provided constant value
 */
void t_md_generator::print_const_value(t_type* type, t_const_value* tvalue) {

  // if tvalue is an indentifier, the constant content is already shown elsewhere
  if (tvalue->get_type() == t_const_value::CV_IDENTIFIER) {
    string fname = program_->get_name() + ".html";
    string name = escape_html( tvalue->get_identifier());
    f_out_ << "`[" << name << "](" << make_file_link(fname) << "#" << to_lower_case(name) << ")`";
    return;
  }
  
  t_type* truetype = type;
  while (truetype->is_typedef()) {
    truetype = ((t_typedef*)truetype)->get_type();
  }
  
  bool first = true;
  if (truetype->is_base_type()) {
    t_base_type::t_base tbase = ((t_base_type*)truetype)->get_base();
    switch (tbase) {
      case t_base_type::TYPE_STRING:
        f_out_ << '"' << escape_html( get_escaped_string(tvalue)) << '"';
        break;
      case t_base_type::TYPE_BOOL:
        f_out_ << ((tvalue->get_integer() != 0) ? "true" : "false");
        break;
      case t_base_type::TYPE_BYTE:
        f_out_ << tvalue->get_integer();
        break;
      case t_base_type::TYPE_I16:
        f_out_ << tvalue->get_integer();
        break;
      case t_base_type::TYPE_I32:
        f_out_ << tvalue->get_integer();
        break;
      case t_base_type::TYPE_I64:
        f_out_ << tvalue->get_integer();
        break;
      case t_base_type::TYPE_DOUBLE:
        if (tvalue->get_type() == t_const_value::CV_INTEGER) {
          f_out_ << tvalue->get_integer();
        } else {
          f_out_ << tvalue->get_double();
        }
        break;
      default:
          f_out_ << "UNKNOWN BASE TYPE";
    }
  } else if (truetype->is_enum()) {
    f_out_ << escape_html(truetype->get_name()) << "." << escape_html(tvalue->get_identifier_name());
  } else if (truetype->is_struct() || truetype->is_xception()) {
    f_out_ << "{ ";
    const vector<t_field*>& fields = ((t_struct*)truetype)->get_members();
    vector<t_field*>::const_iterator f_iter;
    const map<t_const_value*, t_const_value*>& val = tvalue->get_map();
    map<t_const_value*, t_const_value*>::const_iterator v_iter;
    for (v_iter = val.begin(); v_iter != val.end(); ++v_iter) {
      t_type* field_type = NULL;
      for (f_iter = fields.begin(); f_iter != fields.end(); ++f_iter) {
        if ((*f_iter)->get_name() == v_iter->first->get_string()) {
          field_type = (*f_iter)->get_type();
        }
      }
      if (field_type == NULL) {
        throw "type error: " + truetype->get_name() + " has no field " + v_iter->first->get_string();
      }
      if (!first) {
        f_out_ << ", ";
      }
      first = false;
      f_out_ << escape_html( v_iter->first->get_string()) << " = ";
      print_const_value( field_type, v_iter->second);
    }
    f_out_ << " }";
  } else if (truetype->is_map()) {
    f_out_ << "{ ";
    map<t_const_value*, t_const_value*> map_elems = tvalue->get_map();
    map<t_const_value*, t_const_value*>::iterator map_iter;
    for (map_iter = map_elems.begin(); map_iter != map_elems.end(); map_iter++) {
      if (!first) {
        f_out_ << ", ";
      }
      first = false;
      print_const_value( ((t_map*)truetype)->get_key_type(), map_iter->first);
      f_out_ << " = ";
      print_const_value( ((t_map*)truetype)->get_val_type(), map_iter->second);
    }
    f_out_ << " }";
  } else if (truetype->is_list()) {
    f_out_ << "{ ";
    vector<t_const_value*> list_elems = tvalue->get_list();;
    vector<t_const_value*>::iterator list_iter;
    for (list_iter = list_elems.begin(); list_iter != list_elems.end(); list_iter++) {
      if (!first) {
        f_out_ << ", ";
      }
      first = false;
      print_const_value( ((t_list*)truetype)->get_elem_type(), *list_iter);
    }
    f_out_ << " }";
  } else if (truetype->is_set()) {
    f_out_ << "{ ";
    vector<t_const_value*> list_elems = tvalue->get_list();;
    vector<t_const_value*>::iterator list_iter;
    for (list_iter = list_elems.begin(); list_iter != list_elems.end(); list_iter++) {
      if (!first) {
        f_out_ << ", ";
      }
      first = false;
      print_const_value( ((t_set*)truetype)->get_elem_type(), *list_iter);
    }
    f_out_ << " }";
  } else {
    f_out_ << "UNKNOWN TYPE";
  }
}

/**
 * Prints out documentation for arguments/exceptions of a function, if any documentation has been supplied.
 */
void t_md_generator::print_fn_args_doc(t_function* tfunction) {
  bool has_docs = false;
  vector<t_field*> args = tfunction->get_arglist()->get_members();
  vector<t_field*>::iterator arg_iter = args.begin();
  if (arg_iter != args.end()) {
    for ( ; arg_iter != args.end(); arg_iter++) {
      if ((*arg_iter)->has_doc() && !(*arg_iter)->get_doc().empty())
        has_docs = true;
    }
    if (has_docs) {
      arg_iter = args.begin();
      f_out_ << HDR_ARGS << "Parameters" << endl;
      f_out_ << "| Name | Description |";
      for ( ; arg_iter != args.end(); arg_iter++) {
        f_out_ << "| " << (*arg_iter)->get_name();
        f_out_ << " | ";
        f_out_ << (*arg_iter)->get_doc();
        f_out_ << " |" << endl;
      }
    }
  }

  has_docs = false;
  vector<t_field*> excepts = tfunction->get_xceptions()->get_members();
  vector<t_field*>::iterator ex_iter = excepts.begin();
  if (ex_iter != excepts.end()) {
    for ( ; ex_iter != excepts.end(); ex_iter++) {
      if ((*ex_iter)->has_doc() && !(*ex_iter)->get_doc().empty())
        has_docs = true;
    }
    if (has_docs) {
      ex_iter = excepts.begin();
      f_out_ << HDR_ARGS << "Exceptions" << endl;
      f_out_ << "| Type | Description |";
      for ( ; ex_iter != excepts.end(); ex_iter++) {
        f_out_ << "| " << (*ex_iter)->get_type()->get_name();
        f_out_ << " | ";
        f_out_ << (*ex_iter)->get_doc();
        f_out_ << " |" << endl;
      }
    }
  }
}

/**
 * Generates a typedef.
 *
 * @param ttypedef The type definition
 */
void t_md_generator::generate_typedef(t_typedef* ttypedef) {
  string name = ttypedef->get_name();
  f_out_ << HDR_TYPDEF << name << endl;
  f_out_ << "Base type: ";
  print_type(ttypedef->get_type());
  f_out_ << endl << "  * ";
  print_doc(ttypedef);
  f_out_ << endl;
}

/**
 * Generates code for an enumerated type.
 *
 * @param tenum The enumeration
 */
void t_md_generator::generate_enum(t_enum* tenum) {
  string name = tenum->get_name();
  f_out_ << HDR_ENUM << name << endl;
  print_doc(tenum);
  vector<t_enum_value*> values = tenum->get_constants();
  vector<t_enum_value*>::iterator val_iter;
  for (val_iter = values.begin(); val_iter != values.end(); ++val_iter) {
    f_out_ << "* **" << (*val_iter)->get_name() << "**";
    f_out_ << " ` ";
    f_out_ << (*val_iter)->get_value();
    f_out_ << " `:  ";
    print_doc((*val_iter));
    f_out_ << endl;
  }
  f_out_ << endl << endl;
}

/**
 * Generates a constant value
 */
void t_md_generator::generate_const(t_const* tconst) {
  string name = tconst->get_name();
  f_out_ << "* **" << name << "** ";
  print_type(tconst->get_type());
  f_out_ << " ` ";
  print_const_value(tconst->get_type(), tconst->get_value());
  f_out_ << " `:  ";
  print_doc(tconst);
  f_out_ << endl << endl;
}

/**
 * Generates a struct definition for a thrift data type.
 *
 * @param tstruct The struct definition
 */
void t_md_generator::generate_struct(t_struct* tstruct) {
  string name = tstruct->get_name();
  f_out_ << HDR_STRUCT << name << endl;
  print_doc(tstruct);
  vector<t_field*> members = tstruct->get_members();
  vector<t_field*>::iterator mem_iter = members.begin();
  for ( ; mem_iter != members.end(); mem_iter++) {
    f_out_ << "* ` " << (*mem_iter)->get_key() << "` ";
    f_out_ << " **" << (*mem_iter)->get_name() << "**";
    f_out_ << " (";
    print_type((*mem_iter)->get_type());
    f_out_ << ") ";
    if ((*mem_iter)->get_req() == t_field::T_OPTIONAL) {
      f_out_ << " *optional* ";
    } else if ((*mem_iter)->get_req() == t_field::T_REQUIRED) {
      f_out_ << " *required* ";
    } else {
      f_out_ << " *default* ";
    }
    f_out_ << ":  ";
    print_doc((*mem_iter));
    t_const_value* default_val = (*mem_iter)->get_value();
    if (default_val != NULL) {
      f_out_ << "`";
      print_const_value((*mem_iter)->get_type(), default_val);
      f_out_ << "`";
    }
    f_out_ << endl;
  }
}

/**
 * Exceptions are special structs
 *
 * @param tstruct The struct definition
 */
void t_md_generator::generate_xception(t_struct* txception) {
  generate_struct(txception);
}

/**
 * Generates the HTML block for a Thrift service.
 *
 * @param tservice The service definition
 */
void t_md_generator::generate_service(t_service* tservice) {
  f_out_ << HDR_SERVICE << service_name_ << endl;

  if (tservice->get_extends()) {
    f_out_ << "extends ";
    print_type(tservice->get_extends());
    f_out_ << endl;
  }
  print_doc(tservice);
  vector<t_function*> functions = tservice->get_functions();
  vector<t_function*>::iterator fn_iter = functions.begin();
  for ( ; fn_iter != functions.end(); fn_iter++) {
    string fn_name = (*fn_iter)->get_name();
    f_out_ << HDR_FUNCTION << service_name_ << "." << fn_name << endl;
    int offset = print_type((*fn_iter)->get_returntype());
    bool first = true;
    f_out_ << " " << fn_name << "(";
    offset += fn_name.size() + 2;
    vector<t_field*> args = (*fn_iter)->get_arglist()->get_members();
    vector<t_field*>::iterator arg_iter = args.begin();
    for ( ; arg_iter != args.end(); arg_iter++) {
      if (!first) {
        f_out_ << "," << endl;
        for (int i = 0; i < offset; ++i) {
          f_out_ << " ";
        }
      }
      first = false;
      print_type((*arg_iter)->get_type());
      f_out_ << " " << (*arg_iter)->get_name();
      if ((*arg_iter)->get_value() != NULL) {
        f_out_ << " = ";
        print_const_value((*arg_iter)->get_type(), (*arg_iter)->get_value());
      }
    }
    f_out_ << ")" << endl;
    first = true;
    vector<t_field*> excepts = (*fn_iter)->get_xceptions()->get_members();
    vector<t_field*>::iterator ex_iter = excepts.begin();
    if (ex_iter != excepts.end()) {
      f_out_ << "    throws ";
      for ( ; ex_iter != excepts.end(); ex_iter++) {
        if (!first) {
          f_out_ << ", ";
        }
        first = false;
        print_type((*ex_iter)->get_type());
      }
      f_out_ << endl;
    }
    print_doc(*fn_iter);
    print_fn_args_doc(*fn_iter);
  }
}

string t_md_generator::to_lower_case(string name) {
  string s (name);
  std::transform (s.begin(), s.end(), s.begin(), ::tolower);
  return s;
//  return boost::to_lower_copy (name);
}

// trim from start
static std::string ltrim(std::string s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
	return s;
}

// trim from end
static std::string rtrim(std::string s) {
	s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
	return s;
}

// trim from both ends
static std::string trim(std::string s) {
	return ltrim(rtrim(s));
}

THRIFT_REGISTER_GENERATOR(md, "markdown", "")
