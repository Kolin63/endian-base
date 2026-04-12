#include <api.h>

#include "global_api.h"

void hello_world() {
  log_info(global_api_get(), "hello, world!");
}
