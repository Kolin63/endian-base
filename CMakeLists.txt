cmake_minimum_required(VERSION 3.16)

project(endian C)

enable_testing()
add_subdirectory(tests)

file(GLOB SOURCES "src/*.c")

add_library(${PROJECT_NAME} SHARED ${SOURCES})

target_compile_definitions(${PROJECT_NAME} PRIVATE MOD_NAME=${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")

set_target_properties(${PROJECT_NAME} PROPERTIES
  LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/data/endian/plugins"
  C_STANDARD 23
  C_STANDARD_REQUIRED ON
)
