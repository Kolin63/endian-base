#ifndef ENDIAN_BASE_REGMAN_H_
#define ENDIAN_BASE_REGMAN_H_

#include "api.h"

struct end_regman {
  struct registry* player;
};

void end_regman_init(const struct api* api);
void end_regman_cleanup();

struct end_regman* end_regman_get();

struct registry* end_regman_get_player();

#endif
