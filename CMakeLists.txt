cmake_minimum_required(VERSION 3.2)

project(dew)

set(CMAKE_CXX_STANDARD 17)

set(DEW_INCLUDE_DIR include)
set(DEW_HEADERS
)
set(DEW_SOURCES
  src/base.cpp
  src/memory/allocator.cpp
  src/debug.cpp
  src/string.cpp
  src/string_util.cpp
)

add_executable(dew ${DEW_SOURCES})

target_include_directories(dew PUBLIC ${DEW_INCLUDE_DIR})
