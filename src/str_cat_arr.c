#include "str_cat_arr.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

char* str_cat_arr(const char** arr, size_t _size) {
  size_t size = _size / sizeof(char*);
  size_t len = 0;
  for (size_t i = 0; i < size; i++) {
    len += strlen(arr[i]);
  }

  char* str = malloc(len + 1);

  strcpy(str, arr[0]);

  for (size_t i = 1; i < size; i++) {
    strcat(str, arr[i]);
  }

  str[len] = '\0';

  return str;
}
