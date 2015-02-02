
#ifndef __TRANSPORT_H__
#define __TRANSPORT_H__

#include <thrift-nano/defs.h>
#include <thrift-nano/types.h>

typedef struct tn_transport_t
{
    tn_object_t parent;
	bool (*tn_is_open)(struct tn_transport_t *self);
	size_t (*tn_read)(struct tn_transport_t *self, void *buf, size_t len, tn_error_t *error);
	size_t (*tn_write)(struct tn_transport_t *self, void *buf, size_t len, tn_error_t *error);
    size_t (*tn_skip)(struct tn_transport_t *self, size_t len, tn_error_t *error);
} tn_transport_t;
tn_transport_t* tn_transport_init(tn_transport_t *self, tn_error_t *error);
tn_transport_t* tn_transport_create(tn_error_t *error);

#if THRIFT_TRANSPORT_MEMORY
typedef struct tn_transport_memory_t
{
	tn_transport_t parent;
	tn_buffer_t* buf;
} tn_transport_memory_t;
tn_transport_t* tn_transport_memory_init(tn_transport_memory_t *self, size_t bufferSize, tn_error_t *error);
tn_transport_t* tn_transport_memory_create(size_t bufferSize, tn_error_t *error);
#endif

#if THRIFT_TRANSPORT_FILE
typedef struct tn_transport_file_t
{
    tn_transport_t parent;
    FILE *fd;
} tn_transport_file_t;
tn_transport_t* tn_transport_file_init(tn_transport_file_t *self, FILE *fd, tn_error_t *error);
tn_transport_t* tn_transport_file_create(FILE *fd, tn_error_t *error);
#endif

#if THRIFT_PROTOCOL_ASYNC
typedef struct tn_transport_async_t
{
    tn_transport_t parent;
    tn_transport_t *delegate;
    tn_buffer_t *_buf;
    size_t _lastSkipCount;
    size_t _lastWriteCount;
    bool _intx;
} tn_transport_async_t;
tn_transport_t* tn_transport_async_init(tn_transport_async_t *self, tn_error_t *error);
tn_transport_t* tn_transport_async_create(tn_error_t *error);
#endif

#endif
