#ifndef ENDIAN_BASE_STR_CAT_ARR_H_
#define ENDIAN_BASE_STR_CAT_ARR_H_

#include <stddef.h>

// concatenates all strings in arr, puts on heap
char* str_cat_arr(const char** arr, size_t size);

#endif
