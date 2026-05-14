#include "end_api.h"

void stop_cb(struct discord* client, const struct discord_interaction* event) {
  struct discord_interaction_response params = {
      .type = DISCORD_INTERACTION_CHANNEL_MESSAGE_WITH_SOURCE,
      .data = &(struct discord_interaction_callback_data){.content = "Stopping bot!"}};

  discord_create_interaction_response(client, event->id, event->token, &params, NULL);

  log_info("Stopping bot!");
  handle_exit();
}
