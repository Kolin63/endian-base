#ifndef ENDIAN_BASE_END_COM_CITY_INFO_H_
#define ENDIAN_BASE_END_COM_CITY_INFO_H_

#include <concord/jsmn.h>

#include "end_tile_com.h"

enum end_city_size {
  ECS_CAMP,          // 0-49
  ECS_HAMLET,        // 50-99
  ECS_SETTLEMENT,    // 100-499
  ECS_VILLAGE,       // 500-999
  ECS_TOWN,          // 1k-250k
  ECS_CITY,          // 250k-1m
  ECS_METROPOLIS,    // 1m-10m
  ECS_MEGALOPOLIS,   // 10m-250m
  ECS_EPEROPOLIS,    // 250m-1b   (continental)
  ECS_DIPEROPOLIS,   // 1b-5b     (intercontinental)
  ECS_ECUMENOPOLIS,  // 5b+       (global)
};

enum end_city_size end_com_city_info_get_size(unsigned long popl);
const char* end_city_size_get_string(enum end_city_size size);

struct end_com_city_info {
  unsigned long popl;
};

int end_com_city_info_fillout(const char* namespace_name, const char* mod_name,
                              const char* file_name, const jsmntok_t* jsmn,
                              const char* json, struct end_tile_com* com);
char* end_com_city_info_to_json(const struct end_com_city_info* elem);

#endif
