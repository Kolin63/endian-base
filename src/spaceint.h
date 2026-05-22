#ifndef ENDIAN_BASE_SPACE_INT_H_
#define ENDIAN_BASE_SPACE_INT_H_

#include <stddef.h>

typedef unsigned _BitInt(128) spaceint_t;

spaceint_t string_to_spaceint(const char* str);

// format of 0.0e0
// assumes single digit before decimal place
// assumes lowercase e
// assumes positive exponent and positive mantissa
spaceint_t scinot_to_spaceint(const char* str);

// buf should be 64 chars, including null term
// sets first char of buf to null term if buf is too small
void spaceint_to_string(spaceint_t val, char* buf, size_t size);

int spaceint_get_amt_digits(spaceint_t val);

#endif
