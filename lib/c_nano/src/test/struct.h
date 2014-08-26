
#ifndef __STRUCT_H__
#define __STRUCT_H__

#include <protocol.h>
#include <transport.h>

typedef struct
{
	size_t (*tn_write)(void *self, tn_protocol_t *protocol, tn_transport_t *transport);
	size_t (*tn_read)(void *self, tn_protocol_t *protocol, tn_transport_t *transport);
} tn_struct_t;

// generic IO funcs
static size_t tn_write_struct(void *data, tn_protocol_t *protocol, tn_transport_t *transport);
static size_t tn_read_struct(void *data, tn_protocol_t *protocol, tn_transport_t *transport);

#endif
