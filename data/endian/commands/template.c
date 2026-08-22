// https://docs.discord.com/developers/interactions/application-commands

ENDIAN_MIRROR_TAG_START(PREPROC);
#include <stdio.h>
ENDIAN_MIRROR_TAG_END();

ENDIAN_MIRROR_TAG_START(command);
ENDIAN_MIRROR_TAG_DATA(<< dont forget to set command id in tag data >>);
{
  // DISCORD_APPLICATION_CHAT_INPUT
  // DISCORD_APPLICATION_USER
  // DISCORD_APPLICATION_MESSAGE
  .type = DISCORD_APPLICATION_CHAT_INPUT,

  .name = "1-32 characters",

  .description = "Description for CHAT_INPUT commands, 1-100 characters. Empty string for USER and MESSAGE commands",

  .options = {
    .size = 1,
    .options = {
      // DISCORD_APPLICATION_OPTION_SUB_COMMAND
      // DISCORD_APPLICATION_OPTION_SUB_COMMAND_GROUP
      // DISCORD_APPLICATION_OPTION_STRING
      // DISCORD_APPLICATION_OPTION_INTEGER
      // DISCORD_APPLICATION_OPTION_BOOLEAN
      // DISCORD_APPLICATION_OPTION_USER
      // DISCORD_APPLICATION_OPTION_CHANNEL
      // DISCORD_APPLICATION_OPTION_ROLE
      // DISCORD_APPLICATION_OPTION_MENTIONABLE
      // DISCORD_APPLICATION_OPTION_NUMBER
      // DISCORD_APPLICATION_OPTION_ATTACHMENT
      .type = DISCORD_APPLICATION_OPTION_STRING,

      .name = "1-32 character name",
      .description = "1-100 character description",

      .required = false,

      // Choices for the user to pick from, max 25.
      .choices = {
        {
          .name = "1-100 character choice name",

          // Value for the choice, up to 100 characters if string
          // Can be STRING, INTEGER, or NUMBER, but must be formatted as a string
          .value = "500"
        }
      },

      // array of application command option. max 25
      .options = {},

      // The channels shown will be restricted to these types
      // https://docs.discord.com/developers/resources/channel#channel-object-channel-types
      // DISCORD_CHANNEL_GUILD_TEXT
      // DISCORD_CHANNEL_DM
      // DISCORD_CHANNEL_GUILD_VOICE
      // DISCORD_CHANNEL_GROUP_DM
      // DISCORD_CHANNEL_GUILD_CATEGORY
      // DISCORD_CHANNEL_GUILD_ANNOUNCEMENT
      // DISCORD_CHANNEL_GUILD_NEWS
      // DISCORD_CHANNEL_GUILD_STORE
      // DISCORD_CHANNEL_ANNOUNCEMENT_THREAD
      // DISCORD_CHANNEL_GUILD_NEWS_THREAD
      // DISCORD_CHANNEL_GUILD_PUBLIC_THREAD
      // DISCORD_CHANNEL_GUILD_PRIVATE_THREAD
      // DISCORD_CHANNEL_GUILD_STAGE_VOICE
      // DISCORD_CHANNEL_GUILD_DIRECTORY
      // DISCORD_CHANNEL_GUILD_FORUM
      // DISCORD_CHANNEL_GUILD_MEDIA
      .channel_types = {},

      .min_value = "0",
      .max_value = "100",

      // may not be set to true if choices are present.
      .autocomplete = true,
    }
  }

  // Set of permissions represented as a bit set
  // discord.dev -> select app -> Overview -> Bot -> Bot Permissions
  .default_member_permissions = 123,
}
ENDIAN_MIRROR_TAG_END();


// void (struct discord* client, const struct discord_interaction* event)
ENDIAN_MIRROR_TAG_START(command_cb);
ENDIAN_MIRROR_TAG_DATA(ping);
{
  struct discord_interaction_response params = {
      .type = DISCORD_INTERACTION_CHANNEL_MESSAGE_WITH_SOURCE,
      .data = &(struct discord_interaction_callback_data){.content = "hello, world!"}};

  discord_create_interaction_response(client, event->id, event->token, &params, NULL);
}
ENDIAN_MIRROR_TAG_END();
