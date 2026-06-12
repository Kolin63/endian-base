#include "end_com_city_info.h"

#include <stdlib.h>
#include <string.h>

#include "end_api.h"
#include "jsmn_iterator.h"
#include "json_macros.h"

enum end_city_size end_com_city_info_get_size(unsigned long popl) {
  if (popl >= 0 && popl < 50) return ECCIS_CAMP;
  else if (popl >= 50 && popl < 100) return ECCIS_HAMLET;
  else if (popl >= 100 && popl < 500) return ECCIS_SETTLEMENT;
  else if (popl >= 500 && popl < 1000) return ECCIS_VILLAGE;
  else if (popl >= 1000 && popl < 250000) return ECCIS_TOWN;
  else if (popl >= 250000 && popl < 1000000) return ECCIS_CITY;
  else if (popl >= 1000000 && popl < 10000000) return ECCIS_METROPOLIS;
  else if (popl >= 10000000 && popl < 250000000) return ECCIS_MEGALOPOLIS;
  else if (popl >= 250000000 && popl < 1000000000) return ECCIS_EPEROPOLIS;
  else if (popl >= 1000000000 && popl < 5000000000) return ECCIS_DIPEROPOLIS;
  else if (popl >= 5000000000) return ECCIS_ECUMENOPOLIS;
  return ECCIS_ECUMENOPOLIS;
}

const char* end_city_size_get_string(enum end_city_size size) {
  switch (size) {
  case ECCIS_CAMP:
    return "Camp";
  case ECCIS_HAMLET:
    return "Hamlet";
  case ECCIS_SETTLEMENT:
    return "Settlement";
  case ECCIS_VILLAGE:
    return "Village";
  case ECCIS_TOWN:
    return "Town";
  case ECCIS_CITY:
    return "City";
  case ECCIS_METROPOLIS:
    return "Metropolis";
  case ECCIS_MEGALOPOLIS:
    return "Megalopolis";
  case ECCIS_EPEROPOLIS:
    return "Eperopolis";
  case ECCIS_DIPEROPOLIS:
    return "Diperopolis";
  case ECCIS_ECUMENOPOLIS:
    return "Ecumenopolis";
  }
}

int end_com_city_info_fillout(const char* namespace_name, const char* mod_name,
                              const char* file_name, const jsmntok_t* jsmn,
                              const char* json, struct end_tile_com* com) {
  int error = 0;

  if (com->data == NULL) com->data = malloc(sizeof(struct end_tile_com));
  struct end_com_city_info* data = com->data;

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

void end_com_city_info_cleanup(void* elem) {}
