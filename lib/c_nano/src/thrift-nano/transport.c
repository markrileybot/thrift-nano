
#include <thrift-nano/types.h>
#include <thrift-nano/transport.h>
#include <thrift-nano/mem.h>

const static int TN_FT_MAX_CHUNK_SIZE = 256;

static void
tn_transport_destroy(tn_object_t *t)
{
    tn_free(t);
}
static bool
tn_transport_is_open(tn_transport_t *self)
{
	return true;
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
tn_transport_reset(tn_object_t *self)
{
}
tn_transport_t*
tn_transport_init(tn_transport_t *self, tn_error_t *error)
{
    self->parent.tn_destroy = &tn_transport_destroy;
    self->parent.tn_reset = &tn_transport_reset;
	self->tn_is_open = &tn_transport_is_open;
	self->tn_read = &tn_transport_read;
	self->tn_write = &tn_transport_write;
	return self;
}
tn_transport_t*
tn_transport_create(tn_error_t *error)
{
	tn_transport_t *t = tn_alloc(sizeof(tn_transport_t), error);
	if( *error != 0 ) return NULL;
	return tn_transport_init(t, error);
}

#if THRIFT_TRANSPORT_MEMORY
static void
tn_transport_memory_destroy(tn_object_t *t)
{
    tn_object_destroy(((tn_transport_memory_t*)t)->buf);
    tn_free(t);
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
static size_t
tn_transport_memory_skip(tn_transport_t *self, size_t len, tn_error_t *error)
{
    tn_transport_memory_t *mem = (tn_transport_memory_t*) self;
    size_t l = tn_buffer_skip(mem->buf, len);
    if( l != len ) *error = T_ERR_BUFFER_OVERFLOW;
    return l;
}
static void
tn_transport_memory_reset(tn_object_t *self)
{
	tn_object_reset(((tn_transport_memory_t*)self)->buf);
}
tn_transport_t *
tn_transport_memory_init(tn_transport_memory_t *s, size_t bufferSize, tn_error_t *error)
{
	tn_transport_t *self = (tn_transport_t*) s;
    tn_transport_init(self, error);
    self->parent.tn_destroy = &tn_transport_memory_destroy;
    self->parent.tn_reset = &tn_transport_memory_reset;
    self->tn_read = &tn_transport_memory_read;
    self->tn_write = &tn_transport_memory_write;
    self->tn_skip = &tn_transport_memory_skip;
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
#endif

#if THRIFT_TRANSPORT_FILE
static void
tn_transport_file_destroy(tn_object_t *t)
{
    tn_transport_file_t *file = (tn_transport_file_t*)t;
    if( file->fd != NULL )
    {
        fclose(file->fd);
        file->fd = NULL;
    }
    tn_free(t);
}
static size_t
tn_transport_file_read(tn_transport_t *self, void *buf, size_t len, tn_error_t *error)
{
    tn_transport_file_t *file = (tn_transport_file_t*) self;
    size_t l = fread(buf, 1, len, file->fd);
    if( l != len ) *error = T_ERR_BUFFER_UNDERFLOW;
    return l;
}
static size_t
tn_transport_file_write(tn_transport_t *self, void *buf, size_t len, tn_error_t *error)
{
    tn_transport_file_t *file = (tn_transport_file_t*) self;
    size_t l = fwrite(buf, 1, len, file->fd);
    if( l != len ) *error = T_ERR_BUFFER_OVERFLOW;
    return l;
}
static size_t
tn_transport_file_skip(tn_transport_t *self, size_t len, tn_error_t *error)
{
    tn_transport_file_t *file = (tn_transport_file_t*) self;
    if( !fseek(file->fd, len, SEEK_CUR) )
    {
        return len;
    }

    // fd might not support seek...try to read chunks
    const char cbuf[TN_FT_MAX_CHUNK_SIZE];
    void *buf = (void*) &cbuf;
    size_t total = 0;
    while( (total += tn_transport_file_read(self, buf, MIN(total-len, TN_FT_MAX_CHUNK_SIZE), error)) < len )
    {
        if( *error != 0 )
        {
            break;
        }
    }

    return total;
}
tn_transport_t *
tn_transport_file_init(tn_transport_file_t *s, FILE *fd, tn_error_t *error)
{
    tn_transport_t *self = (tn_transport_t*) s;
    tn_transport_init(self, error);
    self->parent.tn_destroy = &tn_transport_file_destroy;
    self->tn_read = &tn_transport_file_read;
    self->tn_write = &tn_transport_file_write;
    self->tn_skip = &tn_transport_file_skip;
    s->fd = fd;
    return self;
}
tn_transport_t*
tn_transport_file_create(FILE *fd, tn_error_t *error)
{
    tn_transport_file_t *t = tn_alloc(sizeof(tn_transport_file_t), error);
    if( *error != 0 ) return NULL;
    return tn_transport_file_init(t, fd, error);
}
#endif



