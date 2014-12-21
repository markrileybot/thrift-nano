
#ifndef __STRUCT_H__
#define __STRUCT_H__

#include <thrift-nano/defs.h>
#include <thrift-nano/transport.h>
#include <thrift-nano/protocol.h>

typedef struct
{
    tn_object_t parent;
	size_t (*tn_write)(void *self, tn_protocol_t *protocol, tn_transport_t *transport, tn_error_t *error);
	size_t (*tn_read)(void *self, tn_protocol_t *protocol, tn_transport_t *transport, tn_error_t *error);
} tn_struct_t;

// generic IO funcs
size_t tn_struct_write(void *data, tn_protocol_t *protocol, tn_transport_t *transport, tn_error_t *error);
size_t tn_struct_read(void *data, tn_protocol_t *protocol, tn_transport_t *transport, tn_error_t *error);

#endif
