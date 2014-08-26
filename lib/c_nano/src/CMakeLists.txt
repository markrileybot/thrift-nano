
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
    mowgli/mowgli_string.c
)

set(TEST_SOURCES
	${SOURCES}
	test/example_gen.c
	test/protocol.c
	test/struct.c
	test/test.c
	test/transport.c
)

add_library(thrift-nano SHARED	${SOURCES})
add_library(thrift-nano-static 	${SOURCES})
add_executable(thrift-nano-test	${TEST_SOURCES})
