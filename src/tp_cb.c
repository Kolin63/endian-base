#include <concord/discord.h>
#include <concord/discord_codecs.h>
#include <stdlib.h>
#include <string.h>

#include "end_api.h"
#include "end_body.h"
#include "end_player.h"
#include "end_pos.h"
#include "fid.h"

struct opt_int {
  unsigned int val;
  bool exists;
};

void tp_cb(struct discord* client, const struct discord_interaction* event) {
  unsigned long uuid = event->member->user->id;
  struct fid sys_fid = {};
  struct fid body_fid = {};
  struct opt_int x = {.val = 0, .exists = false};
  struct opt_int y = {.val = 0, .exists = false};
  struct opt_int z = {.val = 0, .exists = false};

  if (event->data->options != NULL) {
    for (int i = 0; i < event->data->options->size; i++) {
      const char* name = event->data->options->array[i].name;
      char* value = event->data->options->array[i].value;

      if (strcmp(name, "user") == 0) {
        char* str = value;
        uuid = string_to_uuid(str);
      } else if (strcmp(name, "system") == 0) {
        sys_fid = fid_split(value);
        if (sys_fid.ns == NULL) sys_fid.ns = "endian";
      } else if (strcmp(name, "body") == 0) {
        body_fid = fid_split(value);
        if (body_fid.ns == NULL) body_fid.ns = "endian";
      } else if (strcmp(name, "x") == 0) {
        char* str = value;
        x.val = strtoul(str, NULL, 10);
        x.exists = true;
      } else if (strcmp(name, "y") == 0) {
        char* str = value;
        y.val = strtoul(str, NULL, 10);
        y.exists = true;
      } else if (strcmp(name, "z") == 0) {
        char* str = value;
        z.val = strtoul(str, NULL, 10);
        z.exists = true;
      }
    }
  }

  if (sys_fid.id == NULL && body_fid.id == NULL && x.exists == false && y.exists == false && z.exists == false) {
    struct discord_interaction_response params = {
        .type = DISCORD_INTERACTION_CHANNEL_MESSAGE_WITH_SOURCE,
        .data = &(struct discord_interaction_callback_data){
            .flags = DISCORD_MESSAGE_EPHEMERAL,
            .content = "You must provide at least one argument",
        }};

    discord_create_interaction_response(client, event->id, event->token, &params, NULL);
    return;
  }

  struct end_player* player = end_player_get(uuid);
  if (player == NULL) {
    struct discord_interaction_response params = {
        .type = DISCORD_INTERACTION_CHANNEL_MESSAGE_WITH_SOURCE,
        .data = &(struct discord_interaction_callback_data){
            .flags = DISCORD_MESSAGE_EPHEMERAL,
            .content = "Could not get provided player",
        }};

    discord_create_interaction_response(client, event->id, event->token, &params, NULL);
    log_error("Could not get player %zi", uuid);
    return;
  }

  struct end_pos_rich dest = end_pos_get_rich(&player->pos);

  if (sys_fid.id != NULL) {
    const struct end_system* sys = end_system_get(&sys_fid);
    if (sys == NULL) {
      struct discord_interaction_response params = {
          .type = DISCORD_INTERACTION_CHANNEL_MESSAGE_WITH_SOURCE,
          .data = &(struct discord_interaction_callback_data){
              .flags = DISCORD_MESSAGE_EPHEMERAL,
              .content = "System does not exist",
          }};
      discord_create_interaction_response(client, event->id, event->token, &params, NULL);
      return;
    }
    dest.sys = &sys->fid;
  }
  if (body_fid.id != NULL) {
    const struct end_body* body = end_body_get(&body_fid);
    if (body == NULL) {
      struct discord_interaction_response params = {
          .type = DISCORD_INTERACTION_CHANNEL_MESSAGE_WITH_SOURCE,
          .data = &(struct discord_interaction_callback_data){
              .flags = DISCORD_MESSAGE_EPHEMERAL,
              .content = "Body does not exist",
          }};
      discord_create_interaction_response(client, event->id, event->token, &params, NULL);
      return;
    }
    dest.body = &body->fid;
  }
  if (x.exists) dest.x = x.val;
  if (y.exists) dest.y = y.val;
  if (z.exists) dest.z = z.val;

  player->pos = end_pos_rich_get_pos(&dest);

  struct discord_interaction_response params = {
      .type = DISCORD_INTERACTION_CHANNEL_MESSAGE_WITH_SOURCE,
      .data = &(struct discord_interaction_callback_data){
          .flags = DISCORD_MESSAGE_EPHEMERAL,
          .content = "Done",
      }};

  discord_create_interaction_response(client, event->id, event->token, &params, NULL);
}
