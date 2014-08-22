
include_directories( ${CMAKE_CURRENT_BINARY_DIR} )

set(SOURCES
    mowgli/allocation_policy.c
    mowgli/allocator.c
    mowgli/alloc.c
    mowgli/mutex.c
    mowgli/heap.c
    mowgli/dictionary.c
    mowgli/patricia.c
    mowgli/index.c
    mowgli/list.c
    mowgli/class.c
    mowgli/object.c
    mowgli/bootstrap.c
)

add_library(thrift-nano SHARED ${SOURCES})
add_library(thrift-nano-static ${SOURCES})
