#include "end_regman.h"

#include <stdlib.h>

#include "end_api.h"
#include "end_player.h"

struct end_regman* global;

void end_regman_init() {
  global = malloc(sizeof(struct end_regman));

  global->player = registry_init(sizeof(struct end_player*), (void*)end_player_cmp, (void*)end_player_cleanup);
}

void end_regman_cleanup() {
  registry_cleanup(global->player);
  free(global);
}

struct end_regman* end_regman_get() { return global; }

struct registry* end_regman_get_player() { return global->player; }
