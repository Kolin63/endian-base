#include "end_tile_com_resource.h"

#include <string.h>

#include "end_api.h"
#include "json_macros.h"
#include "str_cat_arr.h"

const char* end_resource_type_get_string(enum end_resource_type res) {
  switch (res) {
  case ERT_NONE:
    return "None";
  case ERT_WOOD:
    return "Wood";
  case ERT_STONE:
    return "Stone";
  case ERT_IRON:
    return "Iron";
  case ERT_COAL:
    return "Coal";
  }
}

enum end_resource_type end_resource_type_from_string_id(const char* str) {
  if (strcmp(str, "none") == 0) return ERT_NONE;
  else if (strcmp(str, "wood") == 0) return ERT_WOOD;
  else if (strcmp(str, "stone") == 0) return ERT_STONE;
  else if (strcmp(str, "iron") == 0) return ERT_IRON;
  else if (strcmp(str, "coal") == 0) return ERT_COAL;
  else return ERT_NONE;
}

int end_tile_com_resource_fillout(const char* mod_name, const char* namespace_name,
                                  const char* file_name, const jsmntok_t* jsmn,
                                  const char* json, struct end_tile_com* com) {
  int error = 0;

  struct end_tile_com_resource* res = com->data;

  struct jsmn_iterator iter;
  jsmn_iterator_init(&iter, jsmn, json);

  END_JSON_CHECK_STRING_RET(iter, return error);
  char* str = jsmn_iterator_get_string_heap(json, iter.val);
  res->type = end_resource_type_from_string_id(str);

  return error;
}

char* end_tile_com_resource_to_json(const struct end_tile_com_resource* elem) {
  const char* arr[] = {
      "\"",
      end_resource_type_get_string(elem->type),
      "\"",
  };
  return str_cat_arr(arr, sizeof(arr));
}
