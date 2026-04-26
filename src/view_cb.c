#include <api.h>
#include <concord/discord.h>
#include <player.h>
#include <stdlib.h>
#include <string.h>

void view_cb(const struct api* api, struct discord* client, const struct discord_interaction* event) {
  unsigned long uuid = 0;

  if (event->data->options != NULL) {
    char* uuid_str = NULL;
    for (int i = 0; i < event->data->options->size; i++) {
      const char* name = event->data->options->array[i].name;
      char* value = event->data->options->array[i].value;

      if (strcmp(name, "user") == 0) uuid_str = value;
    }

    if (uuid_str == NULL) {
      log_error(api, "Could not get UUID argument");
      return;
    }

    uuid = strtoul(uuid_str, NULL, 10);
  } else {
    uuid = event->member->user->id;
  }

  struct player* player = api->player_get(uuid);

  if (player == NULL) {
    log_error(api, "Could not get player %zi", uuid);
    return;
  }

  struct discord_embed embeds[] = {
      {
          .title = player->username,
          .description = "Interesting, a description!",
          .color = 0x3498DB,
          .thumbnail =
              &(struct discord_embed_thumbnail){
                  .url = player->avatar,
              },
      },
  };

  struct discord_interaction_response params = {
      .type = DISCORD_INTERACTION_CHANNEL_MESSAGE_WITH_SOURCE,
      .data = &(struct discord_interaction_callback_data){.embeds = &(struct discord_embeds){
                                                              .size = sizeof(embeds) / sizeof *embeds,
                                                              .array = embeds,
                                                          }}};

  api->discord_create_interaction_response(client, event->id, event->token, &params, NULL);
  log_info(api, "Pong!");
}
