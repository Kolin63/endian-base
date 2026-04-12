#include <api.h>
#include <concord/discord.h>
#include <concord/discord_codecs.h>
#include <string.h>

void foobar_cb(struct api* api, struct discord* client,
               const struct discord_interaction* event) {
  char* content = "self";

  struct discord_application_command_interaction_data_options* opts =
      event->data->options->array[0].options->array[0].options;

  for (int i = 0; i < opts->size; ++i) {
    const char* name = opts->array[i].name;
    char* value = opts->array[i].value;

    if (strcmp(name, "user") == 0) content = value;
  }

  struct discord_interaction_response params = {
      .type = DISCORD_INTERACTION_CHANNEL_MESSAGE_WITH_SOURCE,
      .data = &(struct discord_interaction_callback_data){.content = content}};
  api->discord_create_interaction_response(client, event->id, event->token,
                                           &params, NULL);
}
