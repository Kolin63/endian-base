#ifndef ENDIAN_BASE_REGMAN_H_
#define ENDIAN_BASE_REGMAN_H_

#include "registry.h"

struct end_regman {
  struct registry tile_com_tag;
  struct registry tile_com;
  struct registry tile;
  struct registry system;
  struct registry body;
  struct registry player;
};

void end_regman_init();
void end_regman_cleanup();

struct end_regman* end_regman_get();

struct registry* end_regman_get_tile_com_tag();
struct registry* end_regman_get_tile_com();
struct registry* end_regman_get_tile();
struct registry* end_regman_get_system();
struct registry* end_regman_get_body();
struct registry* end_regman_get_player();

#endif
