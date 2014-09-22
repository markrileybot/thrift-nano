
#include <thrift/types.h>

#ifndef __THRIFT_MEM_H__
#define __THRIFT_MEM_H__

void * tn_alloc(size_t size, tn_error_t *error);
void * tn_alloc_array(size_t size, size_t count, tn_error_t *error);
void tn_free(void *m);

#endif
