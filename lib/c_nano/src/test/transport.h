
#ifndef __TRANSPORT_H__
#define __TRANSPORT_H__

#include <mowgli/mowgli.h>

typedef struct tn_transport_t
{
	bool (*tn_is_open)(struct tn_transport_t *self);
	size_t (*tn_read)(struct tn_transport_t *self, void *buf, size_t len);
	size_t (*tn_write)(struct tn_transport_t *self, void *buf, size_t len);
} tn_transport_t;
tn_transport_t* tn_transport_init(tn_transport_t *self);
tn_transport_t* tn_transport_create();

typedef struct tn_transport_memory_t
{
	tn_transport_t parent;
	void *buf;
	size_t pos;
	size_t len;
	void (*tn_reset)(struct tn_transport_t* self);
} tn_transport_memory_t;
tn_transport_t* tn_transport_memory_init(tn_transport_memory_t *self);
tn_transport_t* tn_transport_memory_create(size_t bufferSize);

#endif
