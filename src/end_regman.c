#include "end_regman.h"

#include <stdlib.h>

#include "api.h"
#include "end_player.h"
#include "global_api.h"

struct end_regman* global;

void end_regman_init(const struct api* api) {
  global = malloc(sizeof(struct end_regman));

  global->player = api->registry_init(sizeof(struct end_player*), (void*)end_player_cmp, (void*)end_player_cleanup);
}

void end_regman_cleanup() {
  const struct api* api = global_api_get();
  api->registry_cleanup(global->player);
  free(global);
}

struct end_regman* end_regman_get() { return global; }

struct registry* end_regman_get_player() { return global->player; }
