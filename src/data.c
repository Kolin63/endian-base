#include <concord/discord_codecs.h>
#include <endapi/save.h>
#include <log.h>
#include <stdlib.h>

void load_data() {
  log_info("Loading data from endian!");
  // if (end_body_post_load_fillout() != 0) {
  //   abort_cleanup(EXIT_FAILURE);
  // }
}

void save_data() {
  log_info("Saving data from endian!");
  // end_system_save_all();
  // end_player_save_all();
  log_info("Done saving data from endian!");
}
