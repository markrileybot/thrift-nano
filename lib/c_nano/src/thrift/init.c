
#include <thrift/init.h>
#include <thrift/mem.h>

TN_BOOTSTRAP_FUNC(tn_init)
{
    // init allocator
    tn_alloc_init();
}