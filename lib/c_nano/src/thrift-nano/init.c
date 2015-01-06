
#include <thrift-nano/init.h>
#include <thrift-nano/mem.h>

#ifdef TN_OS_UNKNOWN
void tn_init(void)
#else
TN_BOOTSTRAP_FUNC(tn_init)
#endif
{
    // init allocator
    tn_alloc_init();
}