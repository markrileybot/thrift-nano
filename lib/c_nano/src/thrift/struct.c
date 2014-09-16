
#include <thrift/struct.h>

// generic IO funcs
size_t
tn_write_struct(void *data, tn_protocol_t *protocol, tn_transport_t *transport, tn_error_t *error)
{
	tn_struct_t *s = (tn_struct_t*) data;
	return s->tn_write(data, protocol, transport, error);
}

size_t
tn_read_struct(void *data, tn_protocol_t *protocol, tn_transport_t *transport, tn_error_t *error)
{
	tn_struct_t *s = (tn_struct_t*) data;
	return s->tn_read(data, protocol, transport, error);
}

