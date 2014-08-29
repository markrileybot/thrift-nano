
#include <protocol.h>
#include <struct.h>

// default protocol
static size_t tn_protocol_write_field_begin(tn_protocol_t *self, tn_transport_t *transport, tn_type_t fieldType, int16_t fieldId) {return 0;}
static size_t tn_protocol_write_field_end(tn_protocol_t *self, tn_transport_t *transport) {return 0;}
static size_t tn_protocol_write_field_stop(tn_protocol_t *self, tn_transport_t *transport) {return 0;}
static size_t tn_protocol_write_struct_begin(tn_protocol_t *self, tn_transport_t *transport, void *s) {return 0;}
static size_t tn_protocol_write_struct_end(tn_protocol_t *self, tn_transport_t *transport) {return 0;}
static size_t tn_protocol_write_list_begin(tn_protocol_t *self, tn_transport_t *transport, tn_type_t elemType, int32_t size) {return 0;}
static size_t tn_protocol_write_list_end(tn_protocol_t *self, tn_transport_t *transport) {return 0;}
static size_t tn_protocol_write_map_begin(tn_protocol_t *self, tn_transport_t *transport, tn_type_t keyType, tn_type_t valueType, int32_t size) {return 0;}
static size_t tn_protocol_write_map_end(tn_protocol_t *self, tn_transport_t *transport) {return 0;}
static size_t tn_protocol_write_bytes(tn_protocol_t *self, tn_transport_t *transport, void *s, int32_t len) {return 0;}
static size_t tn_protocol_write_string(tn_protocol_t *self, tn_transport_t *transport, mowgli_string_t *v) {return 0;}
static size_t tn_protocol_write_int16(tn_protocol_t *self, tn_transport_t *transport, int16_t v) {return 0;}
static size_t tn_protocol_write_int32(tn_protocol_t *self, tn_transport_t *transport, int32_t v) {return 0;}
static size_t tn_protocol_write_int64(tn_protocol_t *self, tn_transport_t *transport, int64_t v) {return 0;}
static size_t tn_protocol_write_byte(tn_protocol_t *self, tn_transport_t *transport, int8_t v) {return 0;}
static size_t tn_protocol_write_double(tn_protocol_t *self, tn_transport_t *transport, double v) {return 0;}
static size_t tn_protocol_read_field_begin(tn_protocol_t *self, tn_transport_t *transport, tn_type_t *fieldType, int16_t *fieldId) {return 0;}
static size_t tn_protocol_read_field_end(tn_protocol_t *self, tn_transport_t *transport) {return 0;}
static size_t tn_protocol_read_field_stop(tn_protocol_t *self, tn_transport_t *transport) {return 0;}
static size_t tn_protocol_read_struct_begin(tn_protocol_t *self, tn_transport_t *transport) {return 0;}
static size_t tn_protocol_read_struct_end(tn_protocol_t *self, tn_transport_t *transport) {return 0;}
static size_t tn_protocol_read_list_begin(tn_protocol_t *self, tn_transport_t *transport, tn_type_t *elemType, int32_t *size) {return 0;}
static size_t tn_protocol_read_list_end(tn_protocol_t *self, tn_transport_t *transport) {return 0;}
static size_t tn_protocol_read_map_begin(tn_protocol_t *self, tn_transport_t *transport, tn_type_t *keyType, tn_type_t *valueType, int32_t *size) {return 0;}
static size_t tn_protocol_read_map_end(tn_protocol_t *self, tn_transport_t *transport) {return 0;}
static size_t tn_protocol_read_bytes(tn_protocol_t *self, tn_transport_t *transport, void *s, int32_t *len) {return 0;}
static size_t tn_protocol_read_string(tn_protocol_t *self, tn_transport_t *transport, mowgli_string_t *v) {return 0;}
static size_t tn_protocol_read_int16(tn_protocol_t *self, tn_transport_t *transport, int16_t *v) {return 0;}
static size_t tn_protocol_read_int32(tn_protocol_t *self, tn_transport_t *transport, int32_t *v) {return 0;}
static size_t tn_protocol_read_int64(tn_protocol_t *self, tn_transport_t *transport, int64_t *v) {return 0;}
static size_t tn_protocol_read_byte(tn_protocol_t *self, tn_transport_t *transport, int8_t *v) {return 0;}
static size_t tn_protocol_read_double(tn_protocol_t *self, tn_transport_t *transport, double *v) {return 0;}
static size_t tn_protocol_read_type(tn_protocol_t *self, tn_transport_t *transport, tn_type_t *v) {return 0;}

tn_protocol_t*
tn_protocol_init(tn_protocol_t *protocol)
{
	protocol->tn_write_field_begin   = &tn_protocol_write_field_begin;
	protocol->tn_write_field_end     = &tn_protocol_write_field_end;
	protocol->tn_write_field_stop    = &tn_protocol_write_field_stop;
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
	protocol->tn_write_byte          = &tn_protocol_write_byte;
	protocol->tn_write_double        = &tn_protocol_write_double;

	protocol->tn_read_field_begin   = &tn_protocol_read_field_begin;
	protocol->tn_read_field_end     = &tn_protocol_read_field_end;
	protocol->tn_read_field_stop    = &tn_protocol_read_field_stop;
	protocol->tn_read_struct_begin  = &tn_protocol_read_struct_begin;
	protocol->tn_read_struct_end    = &tn_protocol_read_struct_end;
	protocol->tn_read_list_begin    = &tn_protocol_read_list_begin;
	protocol->tn_read_list_end      = &tn_protocol_read_list_end;
	protocol->tn_read_map_begin     = &tn_protocol_read_map_begin;
	protocol->tn_read_map_end       = &tn_protocol_read_map_end;
	protocol->tn_read_bytes         = &tn_protocol_read_bytes;
	protocol->tn_read_string        = &tn_protocol_read_string;
	protocol->tn_read_int16         = &tn_protocol_read_int16;
	protocol->tn_read_int32         = &tn_protocol_read_int32;
	protocol->tn_read_int64         = &tn_protocol_read_int64;
	protocol->tn_read_byte          = &tn_protocol_read_byte;
	protocol->tn_read_double        = &tn_protocol_read_double;
	protocol->tn_read_type          = &tn_protocol_read_type;
	return protocol;
}
tn_protocol_t*
tn_protocol_create()
{
	tn_protocol_t *protocol = malloc(sizeof(tn_protocol_t));
	if( protocol != NULL ) tn_protocol_init(protocol);
	return protocol;
}


// binary protocol
static size_t
tn_protocol_binary_write_field_begin(tn_protocol_t *self, tn_transport_t *transport, tn_type_t fieldType, int16_t fieldId)
{
	if( self->tn_write_byte(self, transport, fieldType) <= 0 ) return -1;
	if( self->tn_write_int16(self, transport, fieldId) <= 0 ) return -1;
	return 3;
}
static size_t
tn_protocol_binary_write_field_stop(tn_protocol_t *self, tn_transport_t *transport)
{
	if( self->tn_write_byte(self, transport, T_STOP) <= 0 ) return -1;
	return 1;
}
static size_t 
tn_protocol_binary_write_struct_begin(tn_protocol_t *self, tn_transport_t *transport, void *s)
{
}
static size_t 
tn_protocol_binary_write_list_begin(tn_protocol_t *self, tn_transport_t *transport, tn_type_t elemType, int32_t size)
{
	if( self->tn_write_byte(self, transport, elemType) <= 0 ) return -1;
	if( self->tn_write_int32(self, transport, size) <= 0 ) return -1;
	return 5;
}
static size_t 
tn_protocol_binary_write_map_begin(tn_protocol_t *self, tn_transport_t *transport, tn_type_t keyType, tn_type_t valueType, int32_t size)
{
	if( self->tn_write_byte(self, transport, keyType) <= 0 ) return -1;
	if( self->tn_write_byte(self, transport, valueType) <= 0 ) return -1;
	if( self->tn_write_int32(self, transport, size) <= 0 ) return -1;
	return 6;
}
static size_t 
tn_protocol_binary_write_bytes(tn_protocol_t *self, tn_transport_t *transport, void *s, int32_t len)
{
	if( self->tn_write_int32(self, transport, len) <= 0 ) return -1;
	if( transport->tn_write(transport, s, len) <= 0 ) return -1;
	return len + 4;
}
static size_t 
tn_protocol_binary_write_string(tn_protocol_t *self, tn_transport_t *transport, mowgli_string_t *v)
{
	return self->tn_write_bytes(self, transport, v->str, v->pos);
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
tn_protocol_binary_write_byte(tn_protocol_t *self, tn_transport_t *transport, int8_t v)
{
	return transport->tn_write(transport, &v, sizeof(int8_t));
}
static size_t 
tn_protocol_binary_write_double(tn_protocol_t *self, tn_transport_t *transport, double v)
{
	return transport->tn_write(transport, &v, sizeof(double));
}
static size_t
tn_protocol_binary_read_field_begin(tn_protocol_t *self, tn_transport_t *transport, tn_type_t *fieldType, int16_t *fieldId)
{
	if( self->tn_read_type(self, transport, fieldType) <= 0 ) return -1;
	if( *fieldType == T_STOP ) return 1;
	if( self->tn_read_int16(self, transport, fieldId) <= 0 ) return -1;
	return 3;
}
static size_t
tn_protocol_binary_read_field_stop(tn_protocol_t *self, tn_transport_t *transport)
{
	if( self->tn_read_byte(self, transport, T_STOP) <= 0 ) return -1;
	return 1;
}
static size_t 
tn_protocol_binary_read_struct_begin(tn_protocol_t *self, tn_transport_t *transport)
{
}
static size_t 
tn_protocol_binary_read_list_begin(tn_protocol_t *self, tn_transport_t *transport, tn_type_t *elemType, int32_t *size)
{
	if( self->tn_read_type(self, transport, elemType) <= 0 ) return -1;
	if( self->tn_read_int32(self, transport, size) <= 0 ) return -1;
	return 5;
}
static size_t 
tn_protocol_binary_read_map_begin(tn_protocol_t *self, tn_transport_t *transport, tn_type_t *keyType, tn_type_t *valueType, int32_t *size)
{
	if( self->tn_read_type(self, transport, keyType) <= 0 ) return -1;
	if( self->tn_read_type(self, transport, valueType) <= 0 ) return -1;
	if( self->tn_read_int32(self, transport, size) <= 0 ) return -1;
	return 6;
}
static size_t 
tn_protocol_binary_read_bytes(tn_protocol_t *self, tn_transport_t *transport, void *s, int32_t *len)
{
	if( self->tn_read_int32(self, transport, len) <= 0 ) return -1;
	if( transport->tn_read(transport, s, *len) <= 0 ) return -1;
	return *len + 4;
}
static size_t 
tn_protocol_binary_read_string(tn_protocol_t *self, tn_transport_t *transport, mowgli_string_t *v)
{
	mowgli_string_reset(v);
	int32_t len;
	char *buf;

	if( self->tn_read_int32(self, transport, &len) <= 0 ) return -1;
	
	buf = (char*) mowgli_alloc(len);
	if( transport->tn_read(transport, buf, len) > 0 )
	{
		mowgli_string_append(v, buf, len);
	}
	else
	{
		len = -5;
	}
	mowgli_free(buf);
	return len + 4;
}
static size_t 
tn_protocol_binary_read_int16(tn_protocol_t *self, tn_transport_t *transport, int16_t *v)
{
	return transport->tn_read(transport, v, sizeof(int16_t));
}
static size_t 
tn_protocol_binary_read_int32(tn_protocol_t *self, tn_transport_t *transport, int32_t *v)
{
	return transport->tn_read(transport, v, sizeof(int32_t));
}
static size_t 
tn_protocol_binary_read_int64(tn_protocol_t *self, tn_transport_t *transport, int64_t *v)
{
	return transport->tn_read(transport, v, sizeof(int64_t));
}
static size_t 
tn_protocol_binary_read_byte(tn_protocol_t *self, tn_transport_t *transport, int8_t *v)
{
	return transport->tn_read(transport, v, sizeof(int8_t));
}
static size_t 
tn_protocol_binary_read_double(tn_protocol_t *self, tn_transport_t *transport, double *v)
{
	return transport->tn_read(transport, v, sizeof(double));
}
static size_t 
tn_protocol_binary_read_type(tn_protocol_t *self, tn_transport_t *transport, tn_type_t *v)
{
	return transport->tn_read(transport, v, sizeof(int8_t));
}
tn_protocol_binary_t*
tn_protocol_binary_init(tn_protocol_binary_t *binproto)
{
	tn_protocol_t *protocol = (tn_protocol_t*) binproto;
	tn_protocol_init(protocol);
	protocol->tn_write_field_begin   = &tn_protocol_binary_write_field_begin;
	protocol->tn_write_field_stop    = &tn_protocol_binary_write_field_stop;
	protocol->tn_write_list_begin    = &tn_protocol_binary_write_list_begin;
	protocol->tn_write_map_begin     = &tn_protocol_binary_write_map_begin;
	protocol->tn_write_bytes         = &tn_protocol_binary_write_bytes;
	protocol->tn_write_string        = &tn_protocol_binary_write_string;
	protocol->tn_write_int16         = &tn_protocol_binary_write_int16;
	protocol->tn_write_int32         = &tn_protocol_binary_write_int32;
	protocol->tn_write_int64         = &tn_protocol_binary_write_int64;
	protocol->tn_write_byte          = &tn_protocol_binary_write_byte;
	protocol->tn_write_double        = &tn_protocol_binary_write_double;

	protocol->tn_read_field_begin   = &tn_protocol_binary_read_field_begin;
	protocol->tn_read_list_begin    = &tn_protocol_binary_read_list_begin;
	protocol->tn_read_map_begin     = &tn_protocol_binary_read_map_begin;
	protocol->tn_read_bytes         = &tn_protocol_binary_read_bytes;
	protocol->tn_read_string        = &tn_protocol_binary_read_string;
	protocol->tn_read_int16         = &tn_protocol_binary_read_int16;
	protocol->tn_read_int32         = &tn_protocol_binary_read_int32;
	protocol->tn_read_int64         = &tn_protocol_binary_read_int64;
	protocol->tn_read_byte          = &tn_protocol_binary_read_byte;
	protocol->tn_read_double        = &tn_protocol_binary_read_double;
	protocol->tn_read_type          = &tn_protocol_binary_read_type;
	return binproto;
}
tn_protocol_binary_t*
tn_protocol_binary_create()
{
	tn_protocol_binary_t *protocol = malloc(sizeof(tn_protocol_binary_t));
	if( protocol != NULL ) tn_protocol_binary_init(protocol);
	return protocol;
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

