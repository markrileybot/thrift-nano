
#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

#include <mowgli/mowgli.h>
#include <transport.h>

typedef struct tn_protocol_t
{

	// containers
	size_t (*tn_write_struct)(struct tn_protocol_t *self, tn_transport_t *transport, void *s);
	size_t (*tn_write_struct_begin)(struct tn_protocol_t *self, tn_transport_t *transport, void *s);
	size_t (*tn_write_struct_end)(struct tn_protocol_t *self, tn_transport_t *transport);
	size_t (*tn_write_list_begin)(struct tn_protocol_t *self, tn_transport_t *transport, mowgli_list_t *list);
	size_t (*tn_write_list_end)(struct tn_protocol_t *self, tn_transport_t *transport);
	size_t (*tn_write_map_begin)(struct tn_protocol_t *self, tn_transport_t *transport, mowgli_dictionary_t *map);
	size_t (*tn_write_map_end)(struct tn_protocol_t *self, tn_transport_t *transport);
	size_t (*tn_write_bytes)(struct tn_protocol_t *self, tn_transport_t *transport, void *s, size_t len);

	// scalars
	size_t (*tn_write_string)(struct tn_protocol_t *self, tn_transport_t *transport, mowgli_string_t *v);
	size_t (*tn_write_int16)(struct tn_protocol_t *self, tn_transport_t *transport, int16_t v);
	size_t (*tn_write_int32)(struct tn_protocol_t *self, tn_transport_t *transport, int32_t v);
	size_t (*tn_write_int64)(struct tn_protocol_t *self, tn_transport_t *transport, int64_t v);
	size_t (*tn_write_char)(struct tn_protocol_t *self, tn_transport_t *transport, char v);
	size_t (*tn_write_byte)(struct tn_protocol_t *self, tn_transport_t *transport, char v);
	size_t (*tn_write_double)(struct tn_protocol_t *self, tn_transport_t *transport, double v);
} tn_protocol_t;
static tn_protocol_t* tn_protocol_init(tn_protocol_t *protocol);
static tn_protocol_t* tn_protocol_create();


// binary protocol
typedef struct tn_protocol_binary_t
{
	tn_protocol_t parent;
} tn_protocol_binary_t;
static tn_protocol_t* tn_protocol_binary_init(tn_protocol_binary_t *protocol);
static tn_protocol_t* tn_protocol_binary_create();

/**

TODO implement me

// compact binary protocol
typedef struct
{
	tn_protocol_binary_t parent;
} tn_protocol_compact_t;
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

#endif
