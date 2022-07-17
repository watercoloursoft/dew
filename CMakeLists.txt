cmake_minimum_required(VERSION 3.2)

project(dew)

set(CMAKE_CXX_STANDARD 17)

if (MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
endif()

set(DEW_INCLUDE_DIR include)
set(DEW_HEADERS
  ${DEW_INCLUDE_DIR}/base.h
  ${DEW_INCLUDE_DIR}/exception.h
  ${DEW_INCLUDE_DIR}/option.h
  ${DEW_INCLUDE_DIR}/result.h
  ${DEW_INCLUDE_DIR}/vec.h
)
set(DEW_SOURCES
  src/test.cpp
)

add_executable(dew ${DEW_SOURCES})

target_include_directories(dew PUBLIC ${DEW_INCLUDE_DIR})
