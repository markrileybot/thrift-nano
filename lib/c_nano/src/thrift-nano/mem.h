#include <thrift-nano/defs.h>
#include <thrift-nano/types.h>

#ifndef __THRIFT_MEM_H__
#define __THRIFT_MEM_H__

typedef struct tn_allocator_t
{
    void * (*tn_alloc)(struct tn_allocator_t *self, size_t size, tn_error_t *error);
    void * (*tn_alloc_array)(struct tn_allocator_t *self, size_t size, size_t count, tn_error_t *error);
    void   (*tn_free)(struct tn_allocator_t *self, void *m);
} tn_allocator_t;

void tn_alloc_init();
void tn_set_allocator(tn_allocator_t *allocator);
void * tn_alloc(size_t size, tn_error_t *error);
void * tn_alloc_array(size_t size, size_t count, tn_error_t *error);
void tn_free(void *m);

#endif
