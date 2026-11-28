#include <log.h>

#include "end_player.h"
#include "end_poi.h"

void
end_init_data() {
  log_info("Initializing data from endian!");

  end_poi_reg_init();
  end_player_reg_init();

  log_info("Done initializing data from endian!");
}

void
end_load_data() {
  log_info("Loading data from endian!");
  log_info("Done loading data from endian!");
}

void
end_save_data() {
  log_info("Saving data from endian!");
  log_info("Done saving data from endian!");
}

void
end_cleanup_data() {
  log_info("Cleaning up data from endian!");

  end_poi_reg_cleanup();
  end_player_reg_cleanup();

  log_info("Done cleaning up data from endian!");
}
