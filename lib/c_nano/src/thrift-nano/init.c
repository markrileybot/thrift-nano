
#include <thrift-nano/init.h>
#include <thrift-nano/mem.h>

TN_BOOTSTRAP_FUNC(tn_init)
{
    // init allocator
    tn_alloc_init();
}