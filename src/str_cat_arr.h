#ifndef ENDIAN_BASE_STR_CAT_ARR_H_
#define ENDIAN_BASE_STR_CAT_ARR_H_

#include <stddef.h>

#define _STR(x) #x
#define STR(x) _STR(x)

// concatenates all strings in arr, puts on heap
// size should be sizeof(arr), not sizeof(arr) / sizeof(char*)
char* str_cat_arr(const char** arr, size_t size);

#endif
