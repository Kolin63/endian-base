// https://docs.discord.com/developers/interactions/application-commands

#mir_tag_start PREPROC
#include <endapi/function.h>

#include "base/data.h"
#mir_tag_end

#mir_tag_start command
#mir_tag_data save
(struct command){
  .type = DISCORD_APPLICATION_CHAT_INPUT,
  .name = "save",
  .description = "Save all data",
  .default_member_permissions = 8,
}
#mir_tag_end

// void (struct discord* client, const struct discord_interaction* event)
#mir_tag_start command_cb
#mir_tag_data save
{
  function_call_save();
  struct discord_interaction_response params = {
    .type = DISCORD_INTERACTION_CHANNEL_MESSAGE_WITH_SOURCE,
    .data = &(struct discord_interaction_callback_data){
      .content = "Done Saving!",
      .flags = DISCORD_MESSAGE_EPHEMERAL}};
  discord_create_interaction_response(client, event->id, event->token, &params, NULL);
}
#mir_tag_end
