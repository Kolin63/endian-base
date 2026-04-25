#include <api.h>

void stop_cb(const struct api* api, struct discord* client,
             const struct discord_interaction* event) {
  struct discord_interaction_response params = {
      .type = DISCORD_INTERACTION_CHANNEL_MESSAGE_WITH_SOURCE,
      .data = &(struct discord_interaction_callback_data){.content =
                                                              "Stopping bot!"}};
  api->discord_create_interaction_response(client, event->id, event->token,
                                           &params, NULL);
  log_info(api, "Stopping bot!");
  api->bot_exit();
}
