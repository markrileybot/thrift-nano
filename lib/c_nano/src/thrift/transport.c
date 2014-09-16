
#include <thrift/types.h>
#include <thrift/transport.h>
#include <thrift/mem.h>

static bool
tn_transport_is_open(tn_transport_t *self)
{
	return false;
}
static size_t
tn_transport_read(tn_transport_t *self, void *buf, size_t len, tn_error_t *error)
{
	return 0;
}
static size_t
tn_transport_write(tn_transport_t *self, void *buf, size_t len, tn_error_t *error)
{
	return 0;
}
static void
tn_transport_reset(tn_transport_t *self)
{
}
tn_transport_t*
tn_transport_init(tn_transport_t *self, tn_error_t *error)
{
	self->tn_is_open = &tn_transport_is_open;
	self->tn_read = &tn_transport_read;
	self->tn_write = &tn_transport_write;
    self->tn_reset = &tn_transport_reset;
	return self;
}

tn_transport_t*
tn_transport_create(tn_error_t *error)
{
	tn_transport_t *t = tn_alloc(sizeof(tn_transport_t), error);
	if( *error != 0 ) return NULL;
	return tn_transport_init(t, error);
}
void
tn_transport_destroy(tn_transport_t* t)
{
	tn_free(t);
}


static bool
tn_transport_memory_is_open(tn_transport_t *self)
{
    return true;
}
static size_t
tn_transport_memory_read(tn_transport_t *self, void *buf, size_t len, tn_error_t *error)
{
	tn_transport_memory_t *mem = (tn_transport_memory_t*) self;
	size_t l = tn_buffer_read(mem->buf, buf, len);
    if( l != len ) *error = T_ERR_BUFFER_UNDERFLOW;
    return l;
}
static size_t
tn_transport_memory_write(tn_transport_t *self, void *buf, size_t len, tn_error_t *error)
{
	tn_transport_memory_t *mem = (tn_transport_memory_t*) self;
	size_t l = tn_buffer_write(mem->buf, buf, len);
    if( l != len ) *error = T_ERR_BUFFER_OVERFLOW;
    return l;
}
static void
tn_transport_memory_reset(tn_transport_t *self)
{
	tn_buffer_reset(((tn_transport_memory_t*)self)->buf);
}
tn_transport_t *
tn_transport_memory_init(tn_transport_memory_t *s, size_t bufferSize, tn_error_t *error)
{
	tn_transport_t *self = (tn_transport_t*) s;
	self->tn_is_open = &tn_transport_memory_is_open;
	self->tn_read = &tn_transport_memory_read;
	self->tn_write = &tn_transport_memory_write;
    self->tn_reset = &tn_transport_memory_reset;
	if( s->buf == NULL )
	{
		s->buf = tn_buffer_create(bufferSize, error);
	}
	return self;
}
tn_transport_t*
tn_transport_memory_create(size_t bufferSize, tn_error_t *error)
{
	tn_transport_memory_t *t = tn_alloc(sizeof(tn_transport_memory_t), error);
	if( *error != 0 ) return NULL;
	return tn_transport_memory_init(t, bufferSize, error);
}
void
tn_transport_memory_destroy(tn_transport_memory_t* t)
{
	tn_buffer_destroy(t->buf);
	tn_free(t);
}




