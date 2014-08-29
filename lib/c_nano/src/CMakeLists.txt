
include_directories( ${CMAKE_CURRENT_BINARY_DIR} )

set(SOURCES
    mowgli/allocation_policy.c
    mowgli/allocator.c
    mowgli/alloc.c
    mowgli/mutex.c
    mowgli/heap.c
    mowgli/dictionary.c
    mowgli/index.c
    mowgli/list.c
    mowgli/bootstrap.c
    mowgli/mowgli_string.c
    mowgli/logger.c
    mowgli/posix_mutexops.c
    mowgli/win32_mutexops.c
    mowgli/null_mutexops.c
	thrift/protocol.c
	thrift/struct.c
	thrift/transport.c
)

set(TEST_SOURCES
	${SOURCES}
	test/example_gen.c
	test/test.c
)

add_library(thrift-nano SHARED	${SOURCES})
add_library(thrift-nano-static 	${SOURCES})

find_package(Threads REQUIRED)
add_executable(thrift-nano-test	${TEST_SOURCES})
target_link_libraries(thrift-nano-test ${CMAKE_THREAD_LIBS_INIT})
