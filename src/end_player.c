#include "end_player.h"

#define JSMN_HEADER
#include <concord/jsmn.h>
#include <endapi/fileio.h>
#include <endapi/save.h>
#include <endapi/user.h>
#include <jsmn_iterator.h>
#include <log.h>
#include <registry.h>
#include <stdlib.h>
#include <string.h>

#include "end_player.h"
#include "str_cat_arr.h"

static struct registry reg;
typedef struct end_player* end_player_ptr;  // used for registry

void
end_player_reg_init() {
  registry_init(&reg, sizeof(end_player_ptr), (void*)end_player_cmp, (void*)end_player_cleanup);
}

void
end_player_reg_cleanup() {
  registry_cleanup(&reg);
}

int
end_player_cmp(const end_player_ptr* a, const end_player_ptr* b) {
  const struct end_player* x = *a;
  const struct end_player* y = *b;
  return user_cmp(&(x->user), &(y->user));
}

void
end_player_cleanup(end_player_ptr* elem) {
  struct end_player* x = *elem;
  free(x);
}

struct end_player*
end_player_init(unsigned long uuid) {
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

  if (registry_add(&reg, &player) == NULL) {
    log_error("Could not initialize player %zi", uuid);
    free(player);
    return NULL;
  }

  end_player_load(player);

  log_info("Initializing player %s (%zi)", player->user->username, uuid);
  return player;
}

struct end_player*
end_player_get(unsigned long uuid) {
  struct user* key_user = &(struct user){.uuid = uuid};
  struct end_player* key = &(struct end_player){.user = key_user};

  end_player_ptr* ret_ptr = registry_ktov(&reg, &key);
  if (ret_ptr == NULL) {
    return end_player_init(uuid);
  }
  return *ret_ptr;
}

void
end_player_load(struct end_player* elem) {
  char uuid[UUID_STR_LEN];
  uuid_to_string(elem->user->uuid, uuid);

  char* json = NULL;
  if (save_read("endian", "players", uuid, "json", &json) != 0) {
    free(json);
    log_info("Creating new player save for %s", uuid);
    elem->coms = (struct end_com_group_endian_player){};
    end_com_group_endian_player_new(&elem->coms);
    return;
  }

  jsmntok_t* jsmn = fileio_read_json(json);

  struct jsmn_iterator iter;
  jsmn_iterator_init(&iter, jsmn, json);

  while (jsmn_iterator_next(&iter)) {
  }

  log_info("Loading player %s (%s)", elem->user->username, uuid);
}

void
end_player_save(const struct end_player* elem) {
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

void
end_player_save_all() {
  for (int i = 0; i < reg.length; i++) {
    const end_player_ptr* elem = registry_itov(&reg, i);
    end_player_save(*elem);
  }
}
