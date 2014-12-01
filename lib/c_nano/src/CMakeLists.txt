
include_directories( ${CMAKE_CURRENT_BINARY_DIR} )
include_directories(${PROJECT_SOURCE_DIR}/lib/c_nano/src/test/gen-c_nano)

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
	test/gen-c_nano/tn_package_name_test_types.c
	test/test.c
)

set(TEST_SERIAL_SOURCES
  ${SOURCES}
  test/gen-c_nano/tn_package_name_test_types.c
  test/test_serial.c
)

add_library(thrift-nano SHARED	${SOURCES})
add_library(thrift-nano-static 	${SOURCES})

find_package(Threads REQUIRED)
add_executable(thrift-nano-test	${TEST_SOURCES})
add_executable(thrift-nano-serial	${TEST_SERIAL_SOURCES})
