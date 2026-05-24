#include <concord/discord.h>
#include <stdlib.h>
#include <string.h>

#include "end_api.h"
#include "end_player.h"
#include "end_pos.h"
#include "str_cat_arr.h"

void view_cb(struct discord* client, const struct discord_interaction* event) {
  unsigned long long uuid = 0;

  if (event->data->options != NULL) {
    char* uuid_str = NULL;
    for (int i = 0; i < event->data->options->size; i++) {
      const char* name = event->data->options->array[i].name;
      char* value = event->data->options->array[i].value;

      if (strcmp(name, "user") == 0) uuid_str = value;
    }

    if (uuid_str == NULL) {
      log_error("Could not get UUID argument");
      return;
    }

    uuid = string_to_uuid(uuid_str);
  } else {
    uuid = event->member->user->id;
  }

  struct end_player* player = end_player_get(uuid);

  if (player == NULL) {
    log_error("Could not get player %zi", uuid);
    return;
  }

  struct discord_embed_field fields[] = {
      {
          .name = "Temporary field",
          .value = "I put this here so I wouldn't forget how to do it",
      },
  };

  char pos[128];
  end_pos_human_readable(pos, sizeof(pos), player->pos);

  const char* desc_array[] = {
      "Interesting, a description!\n",
      pos,
      "\n\n",
      "foobar",
  };

  char* desc = str_cat_arr(desc_array, sizeof(desc_array) / sizeof(*desc_array));

  struct discord_embed embeds[] = {
      {
          .title = player->user->username,
          .description = desc,
          .color = 0x3498DB,
          .fields =
              &(struct discord_embed_fields){
                  .size = sizeof(fields) / sizeof *fields,
                  .array = fields,
              },
          .thumbnail = &(struct discord_embed_thumbnail){
              .url = player->user->avatar,
          },
      },
  };

  struct discord_interaction_response params = {
      .type = DISCORD_INTERACTION_CHANNEL_MESSAGE_WITH_SOURCE,
      .data = &(struct discord_interaction_callback_data){.embeds = &(struct discord_embeds){
                                                              .size = sizeof(embeds) / sizeof *embeds,
                                                              .array = embeds,
                                                          }}};

  discord_create_interaction_response(client, event->id, event->token, &params, NULL);
  free(desc);
}
