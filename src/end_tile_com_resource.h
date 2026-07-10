#ifndef END_TILE_COM_RESOURCE_H_
#define END_TILE_COM_RESOURCE_H_

#define JSMN_HEADER
#include <concord/jsmn.h>

#include "end_tile_com.h"

// TODO: remove enum and instead use item ids
enum end_resource_type {
  ERT_NONE,
  ERT_WOOD,
  ERT_STONE,
  ERT_IRON,
  ERT_COAL,
};

const char* end_resource_type_get_string(enum end_resource_type res);
enum end_resource_type end_resource_type_from_string_id(const char* str);

struct end_tile_com_resource {
  enum end_resource_type type;
};

int end_tile_com_resource_fillout(const char* mod_name, const char* namespace_name,
                                  const char* file_name, const jsmntok_t* jsmn,
                                  const char* json, struct end_tile_com* com);
char* end_tile_com_resource_to_json(const struct end_tile_com_resource* elem);

#endif
