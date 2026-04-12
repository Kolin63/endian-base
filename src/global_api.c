#include "global_api.h"

#include <api.h>

const static struct api* global_api = NULL;

void global_api_set(const struct api* api) { global_api = api; }

const struct api* global_api_get() { return global_api; }
