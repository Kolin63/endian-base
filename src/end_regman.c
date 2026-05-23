#include "end_regman.h"

#include <stdlib.h>

#include "end_api.h"
#include "end_body.h"
#include "end_player.h"
#include "end_system.h"

struct end_regman* global;

void end_regman_init() {
  global = malloc(sizeof(struct end_regman));

  registry_init(&(global->system), sizeof(struct end_system), (void*)end_system_cmp, (void*)end_system_cleanup);
  registry_init(&(global->body), sizeof(struct end_body), (void*)end_body_cmp, (void*)end_body_cleanup);
  registry_init(&(global->player), sizeof(struct end_player*), (void*)end_player_cmp, (void*)end_player_cleanup);
}

void end_regman_cleanup() {
  registry_cleanup(&(global->system));
  registry_cleanup(&(global->body));
  registry_cleanup(&(global->player));
  free(global);
}

struct end_regman* end_regman_get() { return global; }

struct registry* end_regman_get_system() { return &(global->system); }
struct registry* end_regman_get_body() { return &(global->body); }
struct registry* end_regman_get_player() { return &(global->player); }
