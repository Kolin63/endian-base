// https://docs.discord.com/developers/interactions/application-commands

ENDIAN_MIRROR_TAG_START(command);
ENDIAN_MIRROR_TAG_DATA(ping);
{
  .type = DISCORD_APPLICATION_CHAT_INPUT,

  .name = "ping",

  .description = "Tests connection",
}
ENDIAN_MIRROR_TAG_END();


// void (struct discord* client, const struct discord_interaction* event)
ENDIAN_MIRROR_TAG_START(command_cb);
ENDIAN_MIRROR_TAG_DATA(ping);
{
  struct discord_interaction_response params = {
      .type = DISCORD_INTERACTION_CHANNEL_MESSAGE_WITH_SOURCE,
      .data = &(struct discord_interaction_callback_data){.content = "pong!"}};

  discord_create_interaction_response(client, event->id, event->token, &params, NULL);
  log_info("Pong!");
}
ENDIAN_MIRROR_TAG_END();
