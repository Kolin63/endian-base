#include "end_player.h"

#include <concord/jsmn.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <user.h>

#include "end_api.h"
#include "end_player.h"
#include "end_pos.h"
#include "end_regman.h"
#include "str_cat_arr.h"

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
  player->pos = (struct end_pos){};  // TODO: load save data

  pthread_rwlock_wrlock(&lock);

  if (registry_add(end_regman_get_player(), &player) == NULL) {
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
  struct end_player** ret_ptr = registry_ktov(end_regman_get_player(), &key);
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
    if (strcmp(iter.key, "pos") == 0) {
      if (end_pos_fillout("null", "endian", uuid, iter.val, json, &elem->pos) != 0) {
        log_error("Could not parse pos from player %s (%s)", elem->user->username, uuid);
        elem->pos = (struct end_pos){};
      }
    }
  }

  log_info("Loading player %s (%s)", elem->user->username, uuid);
}

void end_player_save(const struct end_player* elem) {
  char* pos = end_pos_to_json(&elem->pos);

  const char* arr[] = {
      "{\"pos\":",
      pos,
      "}",
  };

  char* cat = str_cat_arr(arr, sizeof(arr));

  free(pos);

  char uuid[UUID_STR_LEN];
  uuid_to_string(elem->user->uuid, uuid);

  save_write("endian", "players", uuid, "json", cat);

  free(cat);

  log_info("Saving player %s", uuid);
}

void end_player_save_all() {
  const struct registry* reg = end_regman_get_player();
  for (int i = 0; i < reg->length; i++) {
    const struct end_player** elem = registry_itov(reg, i);
    end_player_save(*elem);
  }
}
