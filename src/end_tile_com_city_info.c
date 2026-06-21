#include "end_tile_com_city_info.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "end_api.h"
#include "jsmn_iterator.h"
#include "json_macros.h"
#include "str_cat_arr.h"

enum end_city_size end_city_info_get_size(unsigned long popl) {
  if (popl >= 0 && popl < 50) return ECS_CAMP;
  else if (popl >= 50 && popl < 100) return ECS_HAMLET;
  else if (popl >= 100 && popl < 500) return ECS_SETTLEMENT;
  else if (popl >= 500 && popl < 1000) return ECS_VILLAGE;
  else if (popl >= 1000 && popl < 250000) return ECS_TOWN;
  else if (popl >= 250000 && popl < 1000000) return ECS_CITY;
  else if (popl >= 1000000 && popl < 10000000) return ECS_METROPOLIS;
  else if (popl >= 10000000 && popl < 250000000) return ECS_MEGALOPOLIS;
  else if (popl >= 250000000 && popl < 1000000000) return ECS_EPEROPOLIS;
  else if (popl >= 1000000000 && popl < 5000000000) return ECS_DIPEROPOLIS;
  else if (popl >= 5000000000) return ECS_ECUMENOPOLIS;
  return ECS_ECUMENOPOLIS;
}

const char* end_city_size_get_string(enum end_city_size size) {
  switch (size) {
  case ECS_CAMP:
    return "Camp";
  case ECS_HAMLET:
    return "Hamlet";
  case ECS_SETTLEMENT:
    return "Settlement";
  case ECS_VILLAGE:
    return "Village";
  case ECS_TOWN:
    return "Town";
  case ECS_CITY:
    return "City";
  case ECS_METROPOLIS:
    return "Metropolis";
  case ECS_MEGALOPOLIS:
    return "Megalopolis";
  case ECS_EPEROPOLIS:
    return "Eperopolis";
  case ECS_DIPEROPOLIS:
    return "Diperopolis";
  case ECS_ECUMENOPOLIS:
    return "Ecumenopolis";
  }
}

int end_tile_com_city_info_fillout(const char* mod_name, const char* namespace_name,
                                   const char* file_name, const jsmntok_t* jsmn,
                                   const char* json, struct end_tile_com* com) {
  int error = 0;

  com->data = malloc(sizeof(struct end_tile_com));
  struct end_tile_com_city_info* data = com->data;

  struct jsmn_iterator iter;
  jsmn_iterator_init(&iter, jsmn, json);

  while (jsmn_iterator_next(&iter)) {
    if (strcmp(iter.key, "popl") == 0) {
      END_JSON_CHECK_NUMBER(iter);
      char str[32];
      jsmn_iterator_get_string(str, sizeof(str), json, iter.val);
      char* end = NULL;
      data->popl = strtoul(str, &end, 10);
      if (*end != '\0') {
        log_error("Unable to convert city info %s:%s:%s population to int (%s)",
                  mod_name, namespace_name, file_name, str);
      }
    } else {
      log_error("Unknown object %s in end_city_info JSON at %s:%s:%s",
                iter.key, mod_name, namespace_name, file_name);
      error++;
      continue;
    }
  }

  return error;
}

char* end_tile_com_city_info_to_json(const struct end_tile_com_city_info* elem) {
  char popl[32];
  snprintf(popl, sizeof(popl), "%lu", elem->popl);

  const char* arr[] = {
      "{\"popl\":",
      popl,
      "}"};

  return str_cat_arr(arr, sizeof(arr));
}
