
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

static tn_transport_t*
tn_transport_init(tn_transport_t *self)
{
	self->tn_is_open = &tn_transport_is_open;
	self->tn_read = &tn_transport_read;
	self->tn_write = &tn_transport_write;
	return self;
}

static tn_transport_t*
tn_transport_create()
{
	tn_transport_t *t = mowgli_alloc(sizeof(tn_transport_t));
	if( t != NULL ) tn_transport_init(t);
	return t;
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
	len = MIN(len, mem->len - mem->pos);
	memcpy(buf, mem->buf+mem->pos, len);
	mem->pos += len;
    return 0;
}

static size_t
tn_transport_memory_write(tn_transport_t *self, void *buf, size_t len)
{
	tn_transport_memory_t *mem = (tn_transport_memory_t*) self;
	len = MIN(len, mem->len - mem->pos);
	memcpy(mem->buf+mem->pos, buf, len);
	mem->pos += len;
    return len;
}

static void
tn_transport_memory_reset(tn_transport_t *self)
{
	tn_transport_memory_t *mem = (tn_transport_memory_t*) self;
	mem->pos = 0;
}
static tn_transport_t *
tn_transport_memory_init(tn_transport_memory_t *s)
{
	tn_transport_t *self = (tn_transport_t*) s;
	self->tn_is_open = &tn_transport_memory_is_open;
	self->tn_read = &tn_transport_memory_read;
	self->tn_write = &tn_transport_memory_write;
	s->tn_reset = &tn_transport_memory_reset;
	return self;
}

static tn_transport_t*
tn_transport_memory_create(size_t bufferSize)
{
	tn_transport_memory_t *t = mowgli_alloc(sizeof(tn_transport_memory_t));
	if( t == NULL ) return NULL;
	t->buf = mowgli_alloc(bufferSize);
	if( t->buf == NULL )
	{
		mowgli_free(t);
		return NULL;
	}
	tn_transport_memory_init(t);
	return (tn_transport_t*) t;
}





