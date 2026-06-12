#include <stdlib.h>

#include "end_api.h"
#include "end_body.h"
#include "end_system.h"
#include "end_tile_com.h"
#include "save.h"

void load_data() {
  log_info("Loading data from endian-base!");
  rom_load("endian", "tile_coms", end_tile_com_ent_load(file_path, "endian", mod_name, file_name));
  rom_load("endian", "systems", end_system_load(file_path, "endian", mod_name, file_name));
  if (end_body_post_load_fillout() != 0) {
    abort_cleanup(EXIT_FAILURE);
  }
}
