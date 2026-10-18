// https://docs.discord.com/developers/interactions/application-commands

#mir_tag_start PREPROC
#include <log.h>

#include <endapi/exit.h>
#mir_tag_end

#mir_tag_start command
#mir_tag_data stop
(struct command){
  .type = DISCORD_APPLICATION_CHAT_INPUT,
  .name = "stop",
  .description = "Stop the bot",
  .default_member_permissions = 8,
}
#mir_tag_end

// void (struct discord* client, const struct discord_interaction* event)
#mir_tag_start command_cb
#mir_tag_data stop
{
  struct discord_interaction_response params = {
    .type = DISCORD_INTERACTION_CHANNEL_MESSAGE_WITH_SOURCE,
    .data = &(struct discord_interaction_callback_data){.content = "Stopping bot!"}};

  discord_create_interaction_response(client, event->id, event->token, &params, NULL);

  log_info("Stopping bot!");
  handle_exit();
}
#mir_tag_end
