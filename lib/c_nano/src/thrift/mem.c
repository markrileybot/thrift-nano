
#include <config.h>
#include <thrift/mem.h>
#include <mowgli/mowgli.h>

void *
tn_alloc(size_t size, tn_error_t *error)
{
#ifdef THRIFT_ALLOC
    void *p = mowgli_alloc(size);
    if( p == NULL ) *error = T_ERR_ALLOC_FAILED;
    return p;
#else
    *error = T_ERR_ALLOC_NOT_SUPPORTED;
    return NULL;
#endif
}
void *
tn_alloc_array(size_t size, size_t count, tn_error_t *error)
{
#ifdef THRIFT_ALLOC
    void *p = mowgli_alloc_array(size, count);
    if( p == NULL ) *error = T_ERR_ALLOC_FAILED;
    return p;
#else
    *error = T_ERR_ALLOC_NOT_SUPPORTED;
    return NULL;
#endif
}

void
tn_free(void *m)
{
    if( m != NULL ) mowgli_free(m);
}