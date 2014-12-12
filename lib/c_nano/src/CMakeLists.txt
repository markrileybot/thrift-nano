
include_directories( ${CMAKE_CURRENT_BINARY_DIR} )
include_directories(${PROJECT_SOURCE_DIR}/lib/c_nano/src/test/gen-c_nano)

set(SOURCES
  thrift-nano/mem.c
  thrift-nano/types.c
  thrift-nano/protocol.c
  thrift-nano/struct.c
  thrift-nano/transport.c
  thrift-nano/init.c
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

install (FILES thrift-nano/init.h thrift-nano/machine.h thrift-nano/mem.h thrift-nano/protocol.h thrift-nano/struct.h thrift-nano/thrift_nano.h thrift-nano/transport.h thrift-nano/types.h ${PROJECT_BINARY_DIR}/thrift-nano/config.h DESTINATION include/thrift-nano )
install (TARGETS thrift-nano thrift-nano-static LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)