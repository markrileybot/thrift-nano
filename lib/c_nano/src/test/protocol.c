
#include <protocol.h>
#include <struct.h>

// default protocol
static size_t tn_protocol_write_struct(tn_protocol_t *self, tn_transport_t *transport, void *s) {return 0;}
static size_t tn_protocol_write_struct_begin(tn_protocol_t *self, tn_transport_t *transport, void *s) {return 0;}
static size_t tn_protocol_write_struct_end(tn_protocol_t *self, tn_transport_t *transport) {return 0;}
static size_t tn_protocol_write_list_begin(tn_protocol_t *self, tn_transport_t *transport, mowgli_list_t *list) {return 0;}
static size_t tn_protocol_write_list_end(tn_protocol_t *self, tn_transport_t *transport) {return 0;}
static size_t tn_protocol_write_map_begin(tn_protocol_t *self, tn_transport_t *transport, mowgli_dictionary_t *map) {return 0;}
static size_t tn_protocol_write_map_end(tn_protocol_t *self, tn_transport_t *transport) {return 0;}
static size_t tn_protocol_write_bytes(tn_protocol_t *self, tn_transport_t *transport, void *s, size_t len) {return 0;}
static size_t tn_protocol_write_string(tn_protocol_t *self, tn_transport_t *transport, mowgli_string_t *v) {return 0;}
static size_t tn_protocol_write_int16(tn_protocol_t *self, tn_transport_t *transport, int16_t v) {return 0;}
static size_t tn_protocol_write_int32(tn_protocol_t *self, tn_transport_t *transport, int32_t v) {return 0;}
static size_t tn_protocol_write_int64(tn_protocol_t *self, tn_transport_t *transport, int64_t v) {return 0;}
static size_t tn_protocol_write_char(tn_protocol_t *self, tn_transport_t *transport, char v) {return 0;}
static size_t tn_protocol_write_byte(tn_protocol_t *self, tn_transport_t *transport, char v) {return 0;}
static size_t tn_protocol_write_double(tn_protocol_t *self, tn_transport_t *transport, double v) {return 0;}
static tn_protocol_t*
tn_protocol_init(tn_protocol_t *protocol)
{
	protocol->tn_write_struct  		 = &tn_protocol_write_struct;
	protocol->tn_write_struct_begin  = &tn_protocol_write_struct_begin;
	protocol->tn_write_struct_end    = &tn_protocol_write_struct_end;
	protocol->tn_write_list_begin    = &tn_protocol_write_list_begin;
	protocol->tn_write_list_end      = &tn_protocol_write_list_end;
	protocol->tn_write_map_begin     = &tn_protocol_write_map_begin;
	protocol->tn_write_map_end       = &tn_protocol_write_map_end;
	protocol->tn_write_bytes         = &tn_protocol_write_bytes;
	protocol->tn_write_string        = &tn_protocol_write_string;
	protocol->tn_write_int16         = &tn_protocol_write_int16;
	protocol->tn_write_int32         = &tn_protocol_write_int32;
	protocol->tn_write_int64         = &tn_protocol_write_int64;
	protocol->tn_write_char          = &tn_protocol_write_char;
	protocol->tn_write_byte          = &tn_protocol_write_byte;
	protocol->tn_write_double        = &tn_protocol_write_double;
	return protocol;
}
static tn_protocol_t*
tn_protocol_create()
{
	tn_protocol_t *protocol = malloc(sizeof(tn_protocol_t));
	if( protocol != NULL ) tn_protocol_init_default(protocol);
	return protocol;
}


// binary protocol
static size_t 
tn_protocol_binary_write_struct(tn_protocol_t *self, tn_transport_t *transport, void *s)
{
	tn_struct_t *t;
	if( s == NULL )
	{
		self->tn_write_byte(self, transport, 0);
	}
	else
	{
		self->tn_write_byte(self, transport, 1);
		t = (tn_struct_t *) s;
		t->tn_write(t, self, transport);
	}
}
static size_t 
tn_protocol_binary_write_struct_begin(tn_protocol_t *self, tn_transport_t *transport, void *s)
{
}
static size_t 
tn_protocol_binary_write_list_begin(tn_protocol_t *self, tn_transport_t *transport, mowgli_list_t *s)
{
	if( s == NULL || s->count <= 0 )
	{
		self->tn_write_byte(self, transport, 0);
	}
	else
	{
		self->tn_write_byte(self, transport, 1);
		self->tn_write_int32(self, transport, s->count);
	}
}
static size_t 
tn_protocol_binary_write_map_begin(tn_protocol_t *self, tn_transport_t *transport, mowgli_dictionary_t *s)
{
	if( s == NULL )
	{
		self->tn_write_byte(self, transport, 0);
	}
	else
	{
		self->tn_write_byte(self, transport, 1);
		// count elems in map
		size_t count;
		self->tn_write_int32(self, transport, count);
	}
}
static size_t 
tn_protocol_binary_write_bytes(tn_protocol_t *self, tn_transport_t *transport, void *s, size_t len)
{
	if( s == NULL || len == 0 )
	{
		self->tn_write_byte(self, transport, 0);
	}
	else
	{
		self->tn_write_byte(self, transport, 1);
		self->tn_write_int32(self, transport, len);
		transport->tn_write(transport, s, len);
	}
}
static size_t 
tn_protocol_binary_write_string(tn_protocol_t *self, tn_transport_t *transport, mowgli_string_t *v)
{
	size_t ret = 0;
	if( v == NULL || v->pos == 0 )
	{
		self->tn_write_byte(self, transport, 0);
	}
	else
	{
		self->tn_write_byte(self, transport, 1);
		self->tn_write_int32(self, transport, v->pos);
		transport->tn_write(transport, v->str, v->pos);
	}
	return ret;
}
static size_t 
tn_protocol_binary_write_int16(tn_protocol_t *self, tn_transport_t *transport, int16_t v)
{
	return transport->tn_write(transport, &v, sizeof(int16_t));
}
static size_t 
tn_protocol_binary_write_int32(tn_protocol_t *self, tn_transport_t *transport, int32_t v)
{
	return transport->tn_write(transport, &v, sizeof(int32_t));
}
static size_t 
tn_protocol_binary_write_int64(tn_protocol_t *self, tn_transport_t *transport, int64_t v)
{
	return transport->tn_write(transport, &v, sizeof(int64_t));
}
static size_t 
tn_protocol_binary_write_char(tn_protocol_t *self, tn_transport_t *transport, char v)
{
	return transport->tn_write(transport, &v, sizeof(char));
}
static size_t 
tn_protocol_binary_write_byte(tn_protocol_t *self, tn_transport_t *transport, char v)
{
	return transport->tn_write(transport, &v, sizeof(char));
}
static size_t 
tn_protocol_binary_write_double(tn_protocol_t *self, tn_transport_t *transport, double v)
{
	return transport->tn_write(transport, &v, sizeof(double));
}
static tn_protocol_t*
tn_protocol_binary_init(tn_protocol_binary_t *binproto)
{
	tn_protocol_t *protocol = (tn_protocol_t*) binproto;
	tn_protocol_init(protocol);
	protocol->tn_write_struct  		 = &tn_protocol_binary_write_struct;
	protocol->tn_write_list_begin    = &tn_protocol_binary_write_list_begin;
	protocol->tn_write_map_begin     = &tn_protocol_binary_write_map_begin;
	protocol->tn_write_bytes         = &tn_protocol_binary_write_bytes;
	protocol->tn_write_string        = &tn_protocol_binary_write_string;
	protocol->tn_write_int16         = &tn_protocol_binary_write_int16;
	protocol->tn_write_int32         = &tn_protocol_binary_write_int32;
	protocol->tn_write_int64         = &tn_protocol_binary_write_int64;
	protocol->tn_write_char          = &tn_protocol_binary_write_char;
	protocol->tn_write_byte          = &tn_protocol_binary_write_byte;
	protocol->tn_write_double        = &tn_protocol_binary_write_double;
	return protocol;
}
static tn_protocol_t*
tn_protocol_binary_create()
{
	tn_protocol_binary_t *protocol = malloc(sizeof(tn_protocol_binary_t));
	if( protocol != NULL ) tn_protocol_binary_init(protocol);
	return (tn_protocol_t*) protocol;
}

/**

TODO implement me

// compact binary protocol
static size_t 
tn_protocol_binary_write_int16(tn_protocol_t *self, tn_transport_t *transport, int16_t v)
{
	return transport->tn_write(transport, &v, 0, sizeof(int16_t));
}
static size_t 
tn_protocol_binary_write_int32(tn_protocol_t *self, tn_transport_t *transport, int32_t v)
{
	return transport->tn_write(transport, &v, 0, sizeof(int32_t));
}
static size_t 
tn_protocol_binary_write_int64(tn_protocol_t *self, tn_transport_t *transport, int64_t v)
{
	return transport->tn_write(transport, &v, 0, sizeof(int64_t));
}
static void 
tn_protocol_compact_init(tn_protocol_compact_t *protocol)
{
	tn_binary_protocol_init((tn_binary_protocol_t*)protocol);
	protocol->tn_write_int16         = &tn_protocol_binary_write_int16;
	protocol->tn_write_int32         = &tn_protocol_binary_write_int32;
	protocol->tn_write_int64         = &tn_protocol_binary_write_int64;
}
static tn_protocol_t*
tn_protocol_compact_create()
{
	tn_protocol_compact_t *protocol = malloc(sizeof(tn_protocol_compact_t));
	if( protocol != NULL ) tn_protocol_compact_init(protocol);
	return (tn_protocol_t*) protocol;
}
*/

