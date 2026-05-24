#include "../src/spaceint.h"

#include <string.h>

#include "test_suite.h"

void test_digit(int n, int digits) {
  int res = spaceint_get_amt_digits(n);
  if (res == digits) {
    test_pass("%i is %i digits\n", n, res);
  } else {
    test_fail("%i is %i digits, got %i\n", n, digits, res);
  }
}

void test_limit() {
  const unsigned long limit64 = 0xffffffffffffffff;
  const spaceint_t limit128 = 0 - 1;
  const unsigned long* a = (const unsigned long*)&limit128;
  const unsigned long* b = a + 1;
  if (*a == limit64 && *b == limit64) {
    test_pass("Limit is OK (%lx %lx)\n", *a, *b);
  } else {
    test_fail("Limit is bad (%lx %lx)\n", *a, *b);
  }
}

void _test_str_to_int(const char* str, spaceint_t n) {
  spaceint_t res = string_to_spaceint(str);
  if (res == n) {
    test_pass("Converted string %s\n", str);
  } else {
    test_fail("Bad string conversion, expected %s\n", str);
  }
}

#define test_str_to_int(n) _test_str_to_int(#n, n);

void _test_int_to_str(spaceint_t n, const char* str) {
  char buf[64];
  spaceint_to_string(n, buf, sizeof(buf));
  if (strcmp(str, buf) == 0) {
    test_pass("Converted int to string %s\n", buf);
  } else {
    test_fail("Converted int to string, got %s, expected %s\n", buf, str);
  }
}

#define test_int_to_str(n) _test_int_to_str(n, #n);

void test_scinot(const char* str, spaceint_t n) {
  char buf[64];
  spaceint_to_string(n, buf, sizeof(buf));

  spaceint_t res = scinot_to_spaceint(str);
  if (res == n) {
    test_pass("Converted sci not %s to %s\n", str, buf);
  } else {
    char wrongbuf[64];
    spaceint_to_string(res, wrongbuf, sizeof(wrongbuf));

    test_fail("Converted sci not %s, got %s, expected %s\n", str, wrongbuf, buf);
  }
}

int main() {
  test_digit(0, 1);
  test_digit(1, 1);
  test_digit(9, 1);
  test_digit(10, 2);
  test_digit(50, 2);
  test_digit(99, 2);
  test_digit(100, 3);
  test_digit(500, 3);
  test_digit(999, 3);
  test_digit(9999, 4);
  test_digit(99999, 5);

  test_limit();

  test_str_to_int(0);
  test_str_to_int(5);
  test_str_to_int(10);
  test_str_to_int(100);
  test_str_to_int(1000);
  test_str_to_int(10000);
  test_str_to_int(99999999999);

  test_int_to_str(0);
  test_int_to_str(5);
  test_int_to_str(10);
  test_int_to_str(100);
  test_int_to_str(1000);
  test_int_to_str(10000);
  test_int_to_str(99999999999);

  test_scinot("1", 1);
  test_scinot("1.0e0", 1);
  test_scinot("1.0e1", 10);
  test_scinot("1.0e2", 100);
  test_scinot("1.2e2", 120);
  test_scinot("1.24e2", 124);
  test_scinot("5.0001e4", 50001);
  test_scinot("5.00000001e4", 50000);
  test_scinot("5.00000001e5", 500000);
  test_scinot("5.97217e24", string_to_spaceint("5972170000000000000000000"));
  test_scinot("1.898125e27", string_to_spaceint("1898125000000000000000000000"));
  test_scinot("1.988475e30", string_to_spaceint("1988475000000000000000000000000"));
  test_scinot("1.0000000e0", 1);
  test_scinot("1.0000001e0", 1);
  test_scinot("1.0000111e0", 1);

  tests_return;
}
