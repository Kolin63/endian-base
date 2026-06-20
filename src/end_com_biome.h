#ifndef END_COM_BIOME_H_
#define END_COM_BIOME_H_

#define JSMN_HEADER
#include <concord/jsmn.h>

#include "end_tile_com.h"

enum end_biome_type {
  EBT_OCEAN,
  EBT_GRASSLANDS,
  EBT_DESERT,
  EBT_FOREST,
  EBT_MOUNTAINS,
  EBT_ICE,
  EBT_SNOWY,
  EBT_JUNGLE,
};

const char* end_biome_type_get_string(enum end_biome_type biome);
enum end_biome_type end_biome_type_from_string_id(const char* str);

struct end_com_biome {
  enum end_biome_type type;
};

int end_com_biome_fillout(const char* namespace_name, const char* mod_name,
                          const char* file_name, const jsmntok_t* jsmn,
                          const char* json, struct end_tile_com* com);
char* end_com_biome_to_json(const struct end_com_biome* elem);

#endif
