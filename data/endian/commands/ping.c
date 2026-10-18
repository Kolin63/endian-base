// https://docs.discord.com/developers/interactions/application-commands

#mir_tag_start command
#mir_tag_data ping
(struct command){
  .type = DISCORD_APPLICATION_CHAT_INPUT,
  .name = "ping",
  .description = "Tests connection",
}
#mir_tag_end

// void (struct discord* client, const struct discord_interaction* event)
#mir_tag_start command_cb
#mir_tag_data ping
{
  struct discord_interaction_response params = {
    .type = DISCORD_INTERACTION_CHANNEL_MESSAGE_WITH_SOURCE,
    .data = &(struct discord_interaction_callback_data){.content = "pong!"}};

  discord_create_interaction_response(client, event->id, event->token, &params, NULL);
}
#mir_tag_end
