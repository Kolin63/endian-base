#include "spaceint.h"

#include <stdlib.h>
#include <string.h>

spaceint_t string_to_spaceint(const char* str) {
  spaceint_t ret = 0;
  const char* ptr = str;
  while (1) {
    if (*ptr < '0' || *ptr > '9') return ret;

    // shift digits to the left
    ret *= 10;

    // set new digit
    ret += *ptr - '0';

    ptr++;
  }
  return ret;
}

spaceint_t scinot_to_spaceint(const char* str) {
  // mantissa
  int m;
  // exponent
  int n;

  // mantissa string
  char* m_str = malloc(strlen(str) + 1);
  strcpy(m_str, str);

  // exponent string
  char* n_str = m_str;

  const char* ptr;

  while (*n_str != 'e' && *n_str != '\0') n_str++;
  if (*n_str == '\0') {
    n = 0;
  } else {
    // split mantissa and exponent string by setting e to \0
    *n_str = '\0';
    n_str++;

    // get exponent
    n = 0;
    ptr = n_str;
    while (*ptr != '\0') {
      // shift left one digit
      n *= 10;
      // set new digit
      n += *ptr - '0';

      ptr++;
    }
  }

  // get mantissa. for each digit after the decimal place, decrease the
  // exponent by 1. this lets us have an integer mantissa

  // first, get first digit
  m = *m_str - '0';

  // set pointer to first digit after decimal place
  ptr = m_str + 2;

  while (*ptr != '\0') {
    // shift left one digit
    m *= 10;
    // set new digit
    m += *ptr - '0';
    // account for change in exponent
    n -= 1;

    ptr++;
  }

  free(m_str);

  // now apply the exponent
  spaceint_t res = m;

  if (n > 0) {
    for (int i = 0; i < n; i++) {
      res *= 10;
    }
  } else if (n < 0) {
    for (int i = 0; i > n; i--) {
      res /= 10;
    }
  }

  return res;
}

void spaceint_to_string(spaceint_t val, char* buf, size_t size) {
  const int amt_digits = spaceint_get_amt_digits(val);

  // char where null terminator goes
  char* end = buf + amt_digits;

  if (end - buf + 1 > size) {
    buf[0] = '\0';
    return;
  }

  *end = '\0';

  char* ptr = end - 1;

  while (ptr >= buf) {
    // set char to last digit
    *ptr = '0' + val % 10;

    // shift number right one digit
    val /= 10;

    // move ptr to next digit left
    ptr--;
  }
}

int spaceint_get_amt_digits(spaceint_t val) {
  int n = 0;
  do {
    val /= 10;
    n++;
  } while (val != 0);
  return n;
}

unsigned long spaceint_sqrt(spaceint_t val) {
  // https://en.wikipedia.org/wiki/Integer_square_root#Algorithm_using_Newton's_method

  if (val <= 1) return val;

  spaceint_t prev2 = 0;
  spaceint_t prev1 = 2;

  while (1) {
    const spaceint_t x1 = (prev1 + (val / prev1)) / 2;

    if (x1 == prev1) return x1;

    if (x1 == prev2 && x1 != prev1) {
      if (prev1 <= x1) return prev1;
      return x1;
    }

    prev2 = prev1;
    prev1 = x1;
  }
}
