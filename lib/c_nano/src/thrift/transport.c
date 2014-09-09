
#include <transport.h>

static bool
tn_transport_is_open(tn_transport_t *self)
{
	return FALSE;	
}

static size_t
tn_transport_read(tn_transport_t *self, void *buf, size_t len)
{
	return 0;
}

static size_t
tn_transport_write(tn_transport_t *self, void *buf, size_t len)
{
	return 0;
}

tn_transport_t*
tn_transport_init(tn_transport_t *self)
{
	self->tn_is_open = &tn_transport_is_open;
	self->tn_read = &tn_transport_read;
	self->tn_write = &tn_transport_write;
	return self;
}

tn_transport_t*
tn_transport_create()
{
	tn_transport_t *t = mowgli_alloc(sizeof(tn_transport_t));
	if( t != NULL ) tn_transport_init(t);
	return t;
}
void
tn_transport_destroy(tn_transport_t* t)
{
	mowgli_free(t);
}


static bool
tn_transport_memory_is_open(tn_transport_t *self)
{
    return TRUE;
}
static size_t
tn_transport_memory_read(tn_transport_t *self, void *buf, size_t len)
{
	tn_transport_memory_t *mem = (tn_transport_memory_t*) self;
	return tn_buffer_read(mem->buf, buf, len);
}
static size_t
tn_transport_memory_write(tn_transport_t *self, void *buf, size_t len)
{
	tn_transport_memory_t *mem = (tn_transport_memory_t*) self;
	return tn_buffer_write(mem->buf, buf, len);
}
static void
tn_transport_memory_reset(tn_transport_memory_t *self)
{
	tn_buffer_reset(self->buf);
}
tn_transport_t *
tn_transport_memory_init(tn_transport_memory_t *s, size_t bufferSize)
{
	tn_transport_t *self = (tn_transport_t*) s;
	self->tn_is_open = &tn_transport_memory_is_open;
	self->tn_read = &tn_transport_memory_read;
	self->tn_write = &tn_transport_memory_write;
	s->tn_reset = &tn_transport_memory_reset;
	if( s->buf == NULL )
	{
		s->buf = tn_buffer_create(bufferSize);
	}
	return self;
}
tn_transport_t*
tn_transport_memory_create(size_t bufferSize)
{
	tn_transport_memory_t *t = mowgli_alloc(sizeof(tn_transport_memory_t));
	if( t == NULL ) return NULL;
	tn_transport_memory_init(t, bufferSize);
	return (tn_transport_t*) t;
}
void
tn_transport_memory_destroy(tn_transport_memory_t* t)
{
	tn_buffer_destroy(t->buf);
	mowgli_free(t);
}




