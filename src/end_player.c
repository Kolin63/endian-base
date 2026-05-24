#include "end_player.h"

#include <pthread.h>
#include <stdlib.h>
#include <user.h>

#include "end_api.h"
#include "end_player.h"
#include "end_pos.h"
#include "end_regman.h"

static pthread_rwlock_t lock = PTHREAD_RWLOCK_INITIALIZER;

int end_player_cmp(struct end_player* const* a, struct end_player* const* b) {
  const struct end_player* x = *a;
  const struct end_player* y = *b;
  return user_cmp(&(x->user), &(y->user));
}

void end_player_cleanup(struct end_player** elem) {
  struct end_player* x = *elem;
  free(x);
}

struct end_player* end_player_init(unsigned long long uuid) {
  struct user* disc = user_get(uuid);
  if (disc == NULL) {
    log_error("Could not initialize player %zi", uuid);
    return NULL;
  }
  if (disc->uuid != uuid) {
    log_error("Given UUID does not match returned UUID (%zi)", disc->uuid);
    return NULL;
  }

  struct end_player* player = malloc(sizeof(struct end_player));
  player->user = disc;
  player->pos = (struct end_pos){};  // TODO: load save data

  pthread_rwlock_wrlock(&lock);

  if (registry_add(end_regman_get_player(), &player) == NULL) {
    log_error("Could not initialize player %zi", uuid);
    free(player);
    pthread_rwlock_unlock(&lock);
    return NULL;
  }

  log_info("Initializing player %s (%zi)", player->user->username, uuid);
  pthread_rwlock_unlock(&lock);
  return player;
}

struct end_player* end_player_get(unsigned long long uuid) {
  struct user* key_user = &(struct user){.uuid = uuid};
  struct end_player* key = &(struct end_player){.user = key_user};

  pthread_rwlock_rdlock(&lock);
  struct end_player** ret_ptr = registry_ktov(end_regman_get_player(), &key);
  if (ret_ptr == NULL) {
    pthread_rwlock_unlock(&lock);
    return end_player_init(uuid);
  }
  pthread_rwlock_unlock(&lock);
  return *ret_ptr;
}
