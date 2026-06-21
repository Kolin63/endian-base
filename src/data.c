#include <concord/discord_codecs.h>
#include <stdlib.h>

#include "end_api.h"
#include "end_body.h"
#include "end_system.h"
#include "end_tile.h"
#include "end_tile_com.h"
#include "save.h"

void load_data() {
  log_info("Loading data from endian!");
  rom_load("endian", "tile_coms", end_tile_com_ent_load(file_path, mod_name, "endian", file_name));
  rom_load("endian", "tiles", end_tile_ent_load(file_path, "endian", mod_name, file_name));
  rom_load("endian", "systems", end_system_load(file_path, "endian", mod_name, file_name));
  if (end_body_post_load_fillout() != 0) {
    abort_cleanup(EXIT_FAILURE);
  }
}

void save_data() {
  log_info("Saving data from endian!");
  end_system_save_all();
  log_info("Done saving data from endian!");
}

void save_cb(struct discord* client, const struct discord_interaction* event) {
  handle_save();
  struct discord_interaction_response params = {
      .type = DISCORD_INTERACTION_CHANNEL_MESSAGE_WITH_SOURCE,
      .data = &(struct discord_interaction_callback_data){
          .content = "Done Saving!",
          .flags = DISCORD_MESSAGE_EPHEMERAL}};
  discord_create_interaction_response(client, event->id, event->token, &params, NULL);
}
