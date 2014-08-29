
#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

#include <thrift_types.h>
#include <mowgli/mowgli.h>
#include <transport.h>

typedef struct tn_protocol_t
{

	// fields and such
	size_t (*tn_write_field_begin)(struct tn_protocol_t *self, tn_transport_t *transport, tn_type_t fieldType, int16_t fieldId);
	size_t (*tn_write_field_end)(struct tn_protocol_t *self, tn_transport_t *transport);
	size_t (*tn_write_field_stop)(struct tn_protocol_t *self, tn_transport_t *transport);

	// containers
	size_t (*tn_write_struct_begin)(struct tn_protocol_t *self, tn_transport_t *transport, void *s);
	size_t (*tn_write_struct_end)(struct tn_protocol_t *self, tn_transport_t *transport);
	size_t (*tn_write_list_begin)(struct tn_protocol_t *self, tn_transport_t *transport, tn_type_t elemType, int32_t size);
	size_t (*tn_write_list_end)(struct tn_protocol_t *self, tn_transport_t *transport);
	size_t (*tn_write_map_begin)(struct tn_protocol_t *self, tn_transport_t *transport, tn_type_t keyType, tn_type_t valueType, int32_t size);
	size_t (*tn_write_map_end)(struct tn_protocol_t *self, tn_transport_t *transport);
	size_t (*tn_write_bytes)(struct tn_protocol_t *self, tn_transport_t *transport, void *s, int32_t len);

	// scalars
	size_t (*tn_write_string)(struct tn_protocol_t *self, tn_transport_t *transport, mowgli_string_t *v);
	size_t (*tn_write_int16)(struct tn_protocol_t *self, tn_transport_t *transport, int16_t v);
	size_t (*tn_write_int32)(struct tn_protocol_t *self, tn_transport_t *transport, int32_t v);
	size_t (*tn_write_int64)(struct tn_protocol_t *self, tn_transport_t *transport, int64_t v);
	size_t (*tn_write_byte)(struct tn_protocol_t *self, tn_transport_t *transport, int8_t v);
	size_t (*tn_write_double)(struct tn_protocol_t *self, tn_transport_t *transport, double v);


	// fields and such
	size_t (*tn_read_field_begin)(struct tn_protocol_t *self, tn_transport_t *transport, tn_type_t *fieldType, int16_t *fieldId);
	size_t (*tn_read_field_end)(struct tn_protocol_t *self, tn_transport_t *transport);
	size_t (*tn_read_field_stop)(struct tn_protocol_t *self, tn_transport_t *transport);

	// containers
	size_t (*tn_read_struct_begin)(struct tn_protocol_t *self, tn_transport_t *transport);
	size_t (*tn_read_struct_end)(struct tn_protocol_t *self, tn_transport_t *transport);
	size_t (*tn_read_list_begin)(struct tn_protocol_t *self, tn_transport_t *transport, tn_type_t *elemType, int32_t *size);
	size_t (*tn_read_list_end)(struct tn_protocol_t *self, tn_transport_t *transport);
	size_t (*tn_read_map_begin)(struct tn_protocol_t *self, tn_transport_t *transport, tn_type_t *keyType, tn_type_t *valueType, int32_t *size);
	size_t (*tn_read_map_end)(struct tn_protocol_t *self, tn_transport_t *transport);
	size_t (*tn_read_bytes)(struct tn_protocol_t *self, tn_transport_t *transport, void *s, int32_t *len);

	// scalars
	size_t (*tn_read_string)(struct tn_protocol_t *self, tn_transport_t *transport, mowgli_string_t *v);
	size_t (*tn_read_int16)(struct tn_protocol_t *self, tn_transport_t *transport, int16_t *v);
	size_t (*tn_read_int32)(struct tn_protocol_t *self, tn_transport_t *transport, int32_t *v);
	size_t (*tn_read_int64)(struct tn_protocol_t *self, tn_transport_t *transport, int64_t *v);
	size_t (*tn_read_byte)(struct tn_protocol_t *self, tn_transport_t *transport, int8_t *v);
	size_t (*tn_read_type)(struct tn_protocol_t *self, tn_transport_t *transport, tn_type_t *v);
	size_t (*tn_read_double)(struct tn_protocol_t *self, tn_transport_t *transport, double *v);

} tn_protocol_t;
tn_protocol_t* tn_protocol_init(tn_protocol_t *protocol);
tn_protocol_t* tn_protocol_create();


// binary protocol
typedef struct tn_protocol_binary_t
{
	tn_protocol_t parent;
} tn_protocol_binary_t;
tn_protocol_binary_t* tn_protocol_binary_init(tn_protocol_binary_t *protocol);
tn_protocol_binary_t* tn_protocol_binary_create();


/**

TODO implement me

// compact binary protocol
typedef struct
{
	tn_protocol_binary_t parent;
} tn_protocol_compact_t;
size_t 
tn_protocol_binary_write_int16(tn_protocol_t *self, tn_transport_t *transport, int16_t v)
{
	return transport->tn_write(transport, &v, 0, sizeof(int16_t));
}
size_t 
tn_protocol_binary_write_int32(tn_protocol_t *self, tn_transport_t *transport, int32_t v)
{
	return transport->tn_write(transport, &v, 0, sizeof(int32_t));
}
size_t 
tn_protocol_binary_write_int64(tn_protocol_t *self, tn_transport_t *transport, int64_t v)
{
	return transport->tn_write(transport, &v, 0, sizeof(int64_t));
}
void 
tn_protocol_compact_init(tn_protocol_compact_t *protocol)
{
	tn_binary_protocol_init((tn_binary_protocol_t*)protocol);
	protocol->tn_write_int16         = &tn_protocol_binary_write_int16;
	protocol->tn_write_int32         = &tn_protocol_binary_write_int32;
	protocol->tn_write_int64         = &tn_protocol_binary_write_int64;
}
tn_protocol_t*
tn_protocol_compact_create()
{
	tn_protocol_compact_t *protocol = malloc(sizeof(tn_protocol_compact_t));
	if( protocol != NULL ) tn_protocol_compact_init(protocol);
	return (tn_protocol_t*) protocol;
}
*/

#endif
