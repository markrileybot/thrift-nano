

// Here's what I think the generated code should look like:
#include <example_gen.h>

// struct b
static size_t
tn_package_name_structb_write(void *data, tn_protocol_t *protocol, tn_transport_t *transport)
{
	// TODO: read/write field begin/end
	tn_package_name_structb_t *s = (tn_package_name_structb_t*) data;
	protocol->tn_write_struct_begin(protocol, transport, data);
	protocol->tn_write_string(protocol, transport, s->strprop);
	protocol->tn_write_byte(protocol, transport, s->v1);
	protocol->tn_write_byte(protocol, transport, s->v2);
	protocol->tn_write_byte(protocol, transport, s->v3);
	protocol->tn_write_int32(protocol, transport, s->v4);
	protocol->tn_write_struct_end(protocol, transport);
}
static size_t
tn_package_name_structb_read(void *data, tn_protocol_t *protocol, tn_transport_t *transport)
{
/*
	tn_package_name_structb_t *s = (tn_package_name_structb_t*) data;
	protocol->tn_read_struct_begin(transport);
	s->strprop = protocol->tn_read_string(s->strprop);
	s->v1 = protocol->tn_read_byte(transport);
	s->v2 = protocol->tn_read_byte(transport);
	s->v3 = protocol->tn_read_byte(transport);
	s->v4 = protocol->tn_read_int32(transport);
	protocol->tn_read_struct_end();
*/
}
static tn_package_name_structb_t*
tn_package_name_structb_init(tn_package_name_structb_t *ret)
{
	ret->strprop = NULL;
	ret->parent.tn_write = tn_package_name_structb_write;
	ret->parent.tn_read = tn_package_name_structb_read;
	return ret;
}
static tn_package_name_structb_t *
tn_package_name_structb_create()
{
	tn_package_name_structb_t *ret = (tn_package_name_structb_t*) mowgli_alloc(sizeof(tn_package_name_structb_t));
	if( ret == NULL ) return NULL;
	return tn_package_name_structb_init(ret);
}
static void
tn_package_name_structb_destroy(tn_package_name_structb_t *ret)
{
	if( ret ) mowgli_free(ret);
}



// struct a
static size_t
tn_package_name_structa_write(void *data, tn_protocol_t *protocol, tn_transport_t *transport)
{
	tn_package_name_structa_t *s = (tn_package_name_structa_t*) data;
	protocol->tn_write_struct_begin(protocol, transport, data);
	protocol->tn_write_string(protocol, transport, s->strprop);
	protocol->tn_write_struct(protocol, transport, s->structprop);
	protocol->tn_write_struct_end(protocol, transport);
}
static size_t
tn_package_name_structa_read(void *data, tn_protocol_t *protocol, tn_transport_t *transport)
{
/*
	tn_package_name_structb_t *s = (tn_package_name_structb_t*) data;
	s->strprop = protocol->tn_read_string(protocol, transport, s->strprop);
	
	// I think we have to pass a reference to something that will
	// have a read() function assigned in case structprop isn't set (NULL)
	// I'm not sure if there is a better way to do this.
	tn_package_name_structb_t tmp;
	tn_package_name_structb_init(&tmp);
	s->structprop = protocol->tn_read_struct(protocol, transport, s->structprop, &tmp);
*/
}
static tn_package_name_structa_t*
tn_package_name_structa_init(tn_package_name_structa_t *ret)
{
	ret->strprop = NULL;
	ret->structprop = NULL;
	ret->parent.tn_write = tn_package_name_structa_write;
	ret->parent.tn_read = tn_package_name_structa_read;
	return ret;
}
static void
tn_package_name_structa_destroy(tn_package_name_structa_t *ret)
{
	if( ret ) mowgli_free(ret);
}
static tn_package_name_structa_t*
tn_package_name_structa_create()
{
	tn_package_name_structa_t *ret = (tn_package_name_structa_t*) mowgli_alloc(sizeof(tn_package_name_structa_t));
	if( ret == NULL ) return NULL;
	return tn_package_name_structa_init(ret);
}


// package/library level init
static void
tn_package_name_init()
{
	// setup some things like write funcs and mowgli init
}

