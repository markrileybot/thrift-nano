
#ifndef __STRUCT_H__
#define __STRUCT_H__

#include <thrift/transport.h>
#include <thrift/protocol.h>

typedef struct
{
	size_t (*tn_write)(void *self, tn_protocol_t *protocol, tn_transport_t *transport, tn_error_t *error);
	size_t (*tn_read)(void *self, tn_protocol_t *protocol, tn_transport_t *transport, tn_error_t *error);
} tn_struct_t;

// generic IO funcs
size_t tn_write_struct(void *data, tn_protocol_t *protocol, tn_transport_t *transport, tn_error_t *error);
size_t tn_read_struct(void *data, tn_protocol_t *protocol, tn_transport_t *transport, tn_error_t *error);

#endif
