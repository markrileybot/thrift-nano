cmake_minimum_required (VERSION 2.6)
project (thrift-nano)
set (THRIFT_NANO_VERSION_MAJOR 0)
set (THRIFT_NANO_VERSION_MINOR 1)
set (THRIFT_NANO_VERSION_PATCH 0)

set(CMAKE_C_FLAGS_RELEASE	"${CMAKE_C_FLAGS_RELEASE} -Os -flto")
set(CMAKE_SHARED_LINKER_FLAGS 	"${CMAKE_SHARED_LINKER_FLAGS} -flto")

configure_file (
  "${PROJECT_SOURCE_DIR}/lib/c_nano/src/config.h.in"
  "${PROJECT_BINARY_DIR}/thrift-nano/config.h"
)

include_directories("${PROJECT_BINARY_DIR}")
include_directories("${PROJECT_SOURCE_DIR}/lib/c_nano/src")
include_directories("${PROJECT_SOURCE_DIR}/lib/c_nano/src/test")
add_subdirectory(lib/c_nano/src)
add_subdirectory(compiler/cpp)
