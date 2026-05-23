#include "end_api.h"
#include "end_system.h"
#include "save.h"

void load_data() {
  log_info("Loading data from endian-base!");
  rom_load("endian", "systems", end_system_load(file_path, "endian", mod_name, file_name));
}
