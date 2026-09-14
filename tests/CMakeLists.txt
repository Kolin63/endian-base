cmake_minimum_required(VERSION 3.16)

add_executable(space_int_test "src/space_int_test.c" "../src/spaceint.c")

add_test(NAME space_int_test COMMAND space_int_test)

set_target_properties(space_int_test PROPERTIES
  C_STANDARD 23
  C_STANDARD_REQUIRED ON
)
