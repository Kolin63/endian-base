#include <api.h>
#include <concord/discord.h>
#include <string.h>

void echo_cb(struct api* api, struct discord* client,
             const struct discord_interaction* event) {
  char* content = "(error)";

  for (int i = 0; i < event->data->options->size; ++i) {
    const char* name = event->data->options->array[i].name;
    char* value = event->data->options->array[i].value;

    if (strcmp(name, "content") == 0) content = value;
  }

  struct discord_interaction_response params = {
      .type = DISCORD_INTERACTION_CHANNEL_MESSAGE_WITH_SOURCE,
      .data = &(struct discord_interaction_callback_data){.content = content}};
  api->discord_create_interaction_response(client, event->id, event->token,
                                           &params, NULL);
}
