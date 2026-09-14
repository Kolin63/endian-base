#include "end_player.h"

#define JSMN_HEADER
#include <concord/jsmn.h>
#include <endapi/fileio.h>
#include <endapi/save.h>
#include <endapi/user.h>
#include <jsmn_iterator.h>
#include <log.h>
#include <pthread.h>
#include <registry.h>
#include <stdlib.h>
#include <string.h>

#include "end_player.h"
#include "str_cat_arr.h"

static struct registry reg;

static pthread_rwlock_t lock = PTHREAD_RWLOCK_INITIALIZER;

void end_player_reg_init() {
  registry_init(&reg, sizeof(struct end_player*), (void*)end_player_cmp, (void*)end_player_cleanup);
}

void end_player_reg_cleanup() {
  registry_cleanup(&reg);
}

int end_player_cmp(struct end_player* const* a, struct end_player* const* b) {
  const struct end_player* x = *a;
  const struct end_player* y = *b;
  return user_cmp(&(x->user), &(y->user));
}

void end_player_cleanup(struct end_player** elem) {
  struct end_player* x = *elem;
  free(x);
}

struct end_player* end_player_init(unsigned long uuid) {
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

  pthread_rwlock_wrlock(&lock);

  if (registry_add(&reg, &player) == NULL) {
    log_error("Could not initialize player %zi", uuid);
    free(player);
    pthread_rwlock_unlock(&lock);
    return NULL;
  }

  end_player_load(player);

  log_info("Initializing player %s (%zi)", player->user->username, uuid);
  pthread_rwlock_unlock(&lock);
  return player;
}

struct end_player* end_player_get(unsigned long uuid) {
  struct user* key_user = &(struct user){.uuid = uuid};
  struct end_player* key = &(struct end_player){.user = key_user};

  pthread_rwlock_rdlock(&lock);
  struct end_player** ret_ptr = registry_ktov(&reg, &key);
  if (ret_ptr == NULL) {
    pthread_rwlock_unlock(&lock);
    return end_player_init(uuid);
  }
  pthread_rwlock_unlock(&lock);
  return *ret_ptr;
}

void end_player_load(struct end_player* elem) {
  char uuid[UUID_STR_LEN];
  uuid_to_string(elem->user->uuid, uuid);

  char* json = NULL;
  if (save_read("endian", "players", uuid, "json", &json) != 0) {
    log_error("Could not load player %s (%s)", elem->user->username, uuid);
    free(json);
    return;
  }

  jsmntok_t* jsmn = fileio_read_json(json);

  struct jsmn_iterator iter;
  jsmn_iterator_init(&iter, jsmn, json);

  while (jsmn_iterator_next(&iter)) {
  }

  log_info("Loading player %s (%s)", elem->user->username, uuid);
}

void end_player_save(const struct end_player* elem) {
  const char* arr[] = {
      "{",
      "}",
  };

  char* cat = str_cat_arr(arr, sizeof(arr));

  char uuid[UUID_STR_LEN];
  uuid_to_string(elem->user->uuid, uuid);

  save_write("endian", "players", uuid, "json", cat);

  free(cat);

  log_info("Saving player %s", uuid);
}

void end_player_save_all() {
  for (int i = 0; i < reg.length; i++) {
    const struct end_player** elem = registry_itov(&reg, i);
    end_player_save(*elem);
  }
}
