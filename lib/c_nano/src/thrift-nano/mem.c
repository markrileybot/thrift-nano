
#include <thrift-nano/config.h>
#include <stdlib.h>
#include <thrift-nano/mem.h>

static tn_allocator_t *alloc;
static tn_allocator_t malloc_allocator;

#if THRIFT_ALLOC
static void *
tn_malloc_alloc(tn_allocator_t *self, size_t size, tn_error_t *error)
{
	return malloc(size);
}
static void *
tn_malloc_alloc_array(tn_allocator_t *self, size_t size, size_t count, tn_error_t *error)
{
    return calloc(count, size);
}
static void
tn_malloc_free(tn_allocator_t *self, void *m)
{
    if( m != NULL ) free(m);
}
#else
static void *
tn_malloc_alloc(tn_allocator_t *self, size_t size, tn_error_t *error)
{
    *error = T_ERR_ALLOC_NOT_SUPPORTED;
    return NULL;
}
static void *
tn_malloc_alloc_array(tn_allocator_t *self, size_t size, size_t count, tn_error_t *error)
{
    *error = T_ERR_ALLOC_NOT_SUPPORTED;
    return NULL;
}
static void
tn_malloc_free(tn_allocator_t *self, void *m)
{
}
#endif

void
tn_alloc_init()
{
    malloc_allocator.tn_alloc = &tn_malloc_alloc;
    malloc_allocator.tn_alloc_array = &tn_malloc_alloc_array;
    malloc_allocator.tn_free = &tn_malloc_free;
    tn_set_allocator(&malloc_allocator);
}
void
tn_set_allocator(tn_allocator_t *allocator)
{
    if( alloc == NULL )
    {
        alloc = allocator;
    }
}
void *
tn_alloc(size_t size, tn_error_t *error)
{
    void *p = alloc->tn_alloc(alloc, size, error);
    if( p == NULL && *error == T_ERR_OK ) *error = T_ERR_ALLOC_FAILED;
    return p;
}
void *
tn_alloc_array(size_t size, size_t count, tn_error_t *error)
{
    void *p = alloc->tn_alloc_array(alloc, size, count, error);
    if( p == NULL && *error == T_ERR_OK ) *error = T_ERR_ALLOC_FAILED;
    return p;
}
void
tn_free(void *m)
{
    if( m != NULL ) alloc->tn_free(alloc, m);
}
