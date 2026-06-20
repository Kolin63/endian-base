#include "end_tile_com_biome.h"

#include <stdlib.h>
#include <string.h>

#include "end_api.h"
#include "jsmn_iterator.h"
#include "json_macros.h"
#include "str_cat_arr.h"

const char* end_biome_type_get_string(enum end_biome_type biome) {
  switch (biome) {
  case EBT_OCEAN:
    return "Ocean";
  case EBT_GRASSLANDS:
    return "Grasslands";
  case EBT_DESERT:
    return "Desert";
  case EBT_FOREST:
    return "Forest";
  case EBT_MOUNTAINS:
    return "Mountains";
  case EBT_ICE:
    return "Ice";
  case EBT_SNOWY:
    return "Snowy";
  case EBT_JUNGLE:
    return "Jungle";
  }
}

enum end_biome_type end_biome_type_from_string_id(const char* str) {
  if (strcmp(str, "ocean") == 0) return EBT_OCEAN;
  else if (strcmp(str, "grasslands") == 0) return EBT_GRASSLANDS;
  else if (strcmp(str, "desert") == 0) return EBT_DESERT;
  else if (strcmp(str, "forest") == 0) return EBT_FOREST;
  else if (strcmp(str, "mountains") == 0) return EBT_MOUNTAINS;
  else if (strcmp(str, "ice") == 0) return EBT_ICE;
  else if (strcmp(str, "snowy") == 0) return EBT_SNOWY;
  else if (strcmp(str, "jungle") == 0) return EBT_JUNGLE;
  else return EBT_OCEAN;
}

int end_tile_com_biome_fillout(const char* namespace_name, const char* mod_name,
                               const char* file_name, const jsmntok_t* jsmn,
                               const char* json, struct end_tile_com* com) {
  int error = 0;

  struct end_tile_com_biome* biome = com->data;

  struct jsmn_iterator iter;
  jsmn_iterator_init(&iter, jsmn, json);

  END_JSON_CHECK_STRING_RET(iter, return error);
  char* str = jsmn_iterator_get_string_heap(json, iter.val);
  biome->type = end_biome_type_from_string_id(str);
  free(str);

  return error;
}

char* end_tile_com_biome_to_json(const struct end_tile_com_biome* elem) {
  const char* arr[] = {
      "\"",
      end_biome_type_get_string(elem->type),
      "\"",
  };

  return str_cat_arr(arr, sizeof(arr));
}
