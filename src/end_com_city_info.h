#ifndef ENDIAN_BASE_END_COM_CITY_INFO_H_
#define ENDIAN_BASE_END_COM_CITY_INFO_H_

#include <concord/jsmn.h>

#include "end_tile_com.h"

enum end_city_size {
  ECCIS_CAMP,          // 0-49
  ECCIS_HAMLET,        // 50-99
  ECCIS_SETTLEMENT,    // 100-499
  ECCIS_VILLAGE,       // 500-999
  ECCIS_TOWN,          // 1k-250k
  ECCIS_CITY,          // 250k-1m
  ECCIS_METROPOLIS,    // 1m-10m
  ECCIS_MEGALOPOLIS,   // 10m-250m
  ECCIS_EPEROPOLIS,    // 250m-1b   (continental)
  ECCIS_DIPEROPOLIS,   // 1b-5b     (intercontinental)
  ECCIS_ECUMENOPOLIS,  // 5b+       (global)
};

enum end_city_size end_com_city_info_get_size(unsigned long popl);
const char* end_city_size_get_string(enum end_city_size size);

struct end_com_city_info {
  unsigned long popl;
};

int end_com_city_info_fillout(const char* namespace_name, const char* mod_name,
                              const char* file_name, const jsmntok_t* jsmn,
                              const char* json, struct end_tile_com* com);
void end_com_city_info_cleanup(void* elem);

#endif
