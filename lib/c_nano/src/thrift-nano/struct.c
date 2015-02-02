
#include <thrift-nano/struct.h>

// generic IO funcs
size_t
tn_struct_write(void *data, tn_protocol_t *protocol, tn_transport_t *transport, tn_error_t *error)
{
	tn_struct_t *s = (tn_struct_t*) data;
	size_t len = s->tn_write(data, protocol, transport, error);
	tn_object_reset(protocol);
	return len;
}

size_t
tn_struct_read(void *data, tn_protocol_t *protocol, tn_transport_t *transport, tn_error_t *error)
{
	tn_struct_t *s = (tn_struct_t*) data;
	size_t len = s->tn_read(data, protocol, transport, error);
	tn_object_reset(protocol);
	return len;
}

