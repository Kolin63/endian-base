#mir_tag_start command
#mir_tag_data killdante
(struct command){
  .type = DISCORD_APPLICATION_CHAT_INPUT,
  .name = "killdante",
  .description = "Kill Dante (only when in slime form)",
  .owner_only = true,
}
#mir_tag_end

// void (struct discord* client, const struct discord_interaction* event)
#mir_tag_start command_cb
#mir_tag_data killdante
{
  struct discord_interaction_response params = {
    .type = DISCORD_INTERACTION_CHANNEL_MESSAGE_WITH_SOURCE,
    .data = &(struct discord_interaction_callback_data){.content = "hello, world!"}};

  discord_create_interaction_response(client, event->id, event->token, &params, NULL);
}
#mir_tag_end
