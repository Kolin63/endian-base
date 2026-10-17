// https://docs.discord.com/developers/interactions/application-commands

#mir_tag_start PREPROC
#include <stdlib.h>

#include "base/end_player.h"
#include "base/end_view_coms.h"
#mir_tag_end

#mir_tag_start command
#mir_tag_data view
(struct command) {
  .type = DISCORD_APPLICATION_CHAT_INPUT,

  .name = "view",
  .description = "View information about a player",

  .options = (struct command_options) {
    .len = 1,
    .arr = (struct command_option[]) {
      (struct command_option) {
        .type = DISCORD_APPLICATION_OPTION_USER,
        .name = "player",
        .description = "The player to view",
        .required = false,
      }
    }
  }
}
#mir_tag_end

// void (struct discord* client, const struct discord_interaction* event)
#mir_tag_start command_cb
#mir_tag_data view
{
  unsigned long uuid = event->member->user->id;

  GET_COMMAND_OPTIONS(event) {
    GET_COMMAND_OPTION("player") {
      uuid = string_to_uuid(value);
    }
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

  char* buf = end_view_coms_gen(player);

  struct discord_embed embeds[] = {
      {
          .title = player->user->username,
          .description = buf,
          .color = 0x3498DB,
          .thumbnail = &(struct discord_embed_thumbnail){
              .url = player->user->avatar,
          },
      },
  };

  struct discord_interaction_response params = {
      .type = DISCORD_INTERACTION_CHANNEL_MESSAGE_WITH_SOURCE,
      .data = &(struct discord_interaction_callback_data){
          .embeds = &(struct discord_embeds){
              .size = sizeof(embeds) / sizeof *embeds,
              .array = embeds,
          }}};

  discord_create_interaction_response(client, event->id, event->token, &params, NULL);

  free(buf);
}
#mir_tag_end
