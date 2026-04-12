#ifndef ENDIAN_GLOBAL_API_H_
#define ENDIAN_GLOBAL_API_H_

#include <api.h>

void global_api_set(const struct api* api);
const struct api* global_api_get();

#endif
