
include_directories( ${CMAKE_CURRENT_BINARY_DIR} )

set(SOURCES
  thrift/mem.c
  thrift/types.c
	thrift/protocol.c
	thrift/struct.c
	thrift/transport.c
	thrift/init.c
)

set(TEST_SOURCES
	${SOURCES}
	test/example_gen.c
	test/test.c
)

set(TEST_SERIAL_SOURCES
  ${SOURCES}
  test/example_gen.c
  test/test_serial.c
)

add_library(thrift-nano SHARED	${SOURCES})
add_library(thrift-nano-static 	${SOURCES})

find_package(Threads REQUIRED)
add_executable(thrift-nano-test	${TEST_SOURCES})
add_executable(thrift-nano-serial	${TEST_SERIAL_SOURCES})
