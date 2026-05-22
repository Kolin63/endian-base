#ifndef ENDIAN_BASE_TEST_SUITE_H_
#define ENDIAN_BASE_TEST_SUITE_H_

#include <stdio.h>

static int tests_passed = 0;
static int tests_total = 0;

enum test_status {
  TEST_PASS,
  TEST_FAIL
};

#define __test_str_line_num(x) #x
#define _test_str_line_num(x) __test_str_line_num(x)
#define test_str_line_num _test_str_line_num(__LINE__)

#define test_log_pass(...) printf("\e[0;90m" __FILE_NAME__ ":" test_str_line_num " \e[0;92mPASS \e[0m" __VA_ARGS__);
#define test_log_fail(...) printf("\e[0;90m" __FILE_NAME__ ":" test_str_line_num " \e[0;91mFAIL \e[0m" __VA_ARGS__);

#define test_pass(...) \
  tests_total++;       \
  tests_passed++;      \
  test_log_pass(__VA_ARGS__);
#define test_fail(...) \
  tests_total++;       \
  test_log_fail(__VA_ARGS__);

#define tests_return                                                          \
  {                                                                           \
    if (tests_passed == tests_total) {                                        \
      test_log_pass("All Tests Passed (%i/%i)\n", tests_passed, tests_total); \
    } else {                                                                  \
      test_log_fail("Failed Tests (%i/%i)\n", tests_passed, tests_total);     \
    }                                                                         \
    return tests_total - tests_passed;                                        \
  }

#endif
