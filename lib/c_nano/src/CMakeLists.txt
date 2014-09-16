
include_directories( ${CMAKE_CURRENT_BINARY_DIR} )

set(SOURCES
  mowgli/allocation_policy.c
  mowgli/allocator.c
  mowgli/alloc.c
  mowgli/bootstrap.c
  mowgli/logger.c
  thrift/mem.c
  thrift/types.c
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
