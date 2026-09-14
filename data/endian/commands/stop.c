// https://docs.discord.com/developers/interactions/application-commands

ENDIAN_MIRROR_TAG_START(PREPROC);
#include <log.h>

#include <endapi/exit.h>
ENDIAN_MIRROR_TAG_END();

ENDIAN_MIRROR_TAG_START(command);
ENDIAN_MIRROR_TAG_DATA(stop);
(struct command){
  .type = DISCORD_APPLICATION_CHAT_INPUT,
  .name = "stop",
  .description = "Stop the bot",
  .default_member_permissions = 8,
}
ENDIAN_MIRROR_TAG_END();


// void (struct discord* client, const struct discord_interaction* event)
ENDIAN_MIRROR_TAG_START(command_cb);
ENDIAN_MIRROR_TAG_DATA(stop);
{
  struct discord_interaction_response params = {
      .type = DISCORD_INTERACTION_CHANNEL_MESSAGE_WITH_SOURCE,
      .data = &(struct discord_interaction_callback_data){.content = "Stopping bot!"}};

  discord_create_interaction_response(client, event->id, event->token, &params, NULL);

  log_info("Stopping bot!");
  handle_exit();
}
ENDIAN_MIRROR_TAG_END();
