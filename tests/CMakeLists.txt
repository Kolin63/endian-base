cmake_minimum_required(VERSION 3.16)

add_executable(space_int_test space_int_test.c)

target_link_libraries(space_int_test PRIVATE endian)

add_test(NAME space_int_test COMMAND space_int_test)

set_target_properties(space_int_test PROPERTIES
  C_STANDARD 23
  C_STANDARD_REQUIRED ON
)
