cmake_minimum_required(VERSION 3.2)

project(dew)

# set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)


set(DEW_INCLUDE_DIR include)
set(DEW_HEADERS
)
set(DEW_SOURCES
  src/base.c

  src/memory/allocator.c
)

add_executable(dew ${DEW_SOURCES})

target_include_directories(dew PUBLIC ${DEW_INCLUDE_DIR})
