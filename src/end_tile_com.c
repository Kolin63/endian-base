#include "end_tile_com.h"

#include <stdlib.h>
#include <string.h>

#include "end_api.h"
#include "end_regman.h"
#include "function.h"
#include "jsmn_iterator.h"
#include "json_macros.h"

int end_tile_com_fillout(const char* namespace_name, const char* mod_name,
                         const char* file_name, const jsmntok_t* jsmn, const char* json,
                         const char* key, struct end_tile_com* com) {
  int error = 0;

  com->data = NULL;

  int i = registry_ktoi(end_regman_get_tile_com(),
                        &(struct end_tile_com_ent){.id = (char*)key, .namespace = namespace_name});
  if (i == -1) {
    log_error("Tile component %s doesn't exist from %s:%s:%s",
              key, mod_name, namespace_name, file_name);
    error++;
    return error;
  }
  com->id = i;

  const struct end_tile_com_ent* ent = registry_itov(end_regman_get_tile_com(), i);
  error += ent->fillout(mod_name, namespace_name, file_name, jsmn, json, com);

  return error;
}

int end_tile_com_cmp(const struct end_tile_com* a, const struct end_tile_com* b) {
  return a->id - b->id;
}

void end_tile_com_cleanup(struct end_tile_com* elem) {
  if (elem->data != NULL) free(elem->data);
}

int end_tile_com_ent_fillout(const char* namespace_name, const char* mod_name,
                             const char* file_name, const jsmntok_t* jsmn,
                             const char* json, struct end_tile_com_ent* com) {
  int error = 0;

  com->namespace = namespace_name;

  struct jsmn_iterator iter;
  jsmn_iterator_init(&iter, jsmn, json);

  while (jsmn_iterator_next(&iter)) {
    if (strcmp(iter.key, "name") == 0) {
      END_JSON_CHECK_STRING(iter);
      com->id = jsmn_iterator_get_string_heap(json, iter.val);
    } else if (strcmp(iter.key, "fillout") == 0) {
      END_JSON_CHECK_STRING(iter);
      char* func_name = jsmn_iterator_get_string_heap(json, iter.val);
      const struct function* func_data = function_get(func_name);
      if (func_data == NULL) {
        log_error("Could not get function %s from %s:%s:%s",
                  func_name, mod_name, namespace_name, file_name);
        error++;
        free(func_name);
        continue;
      }
      if (func_data->type != FT_FILLOUT) {
        log_error("Tile component fillout function is not of type fillout %s:%s:%s func %s",
                  mod_name, namespace_name, file_name, func_name);
        error++;
        free(func_name);
        continue;
      }
      com->fillout = (void*)func_data->function;
      free(func_name);
    } else if (strcmp(iter.key, "cleanup") == 0) {
      END_JSON_CHECK_STRING(iter);
      char* func_name = jsmn_iterator_get_string_heap(json, iter.val);
      const struct function* func_data = function_get(func_name);
      if (func_data == NULL) {
        log_error("Could not get function %s from %s:%s:%s",
                  func_name, mod_name, namespace_name, file_name);
        error++;
        free(func_name);
        continue;
      }
      if (func_data->type != FT_DATA) {
        log_error("Tile component fillout function is not of type data %s:%s:%s func %s",
                  mod_name, namespace_name, file_name, func_name);
        error++;
        free(func_name);
        continue;
      }
      com->cleanup = (void*)func_data->function;
      free(func_name);
    } else if (strcmp(iter.key, "template") == 0) {
      continue;
    } else {
      log_error("Unknown object %s in %s:%s:%s", iter.key, mod_name, namespace_name, file_name);
      error++;
      continue;
    }
  }
  return error;
}

void end_tile_com_ent_load(const char* file_path, const char* namespace_name,
                           const char* mod_name, const char* file_name) {
  if (strcmp(file_name, "template.json") == 0) return;

  FILE* file = fopen(file_path, "r");
  if (file == NULL) {
    log_error("Could not open %s from %s:%s", file_path, mod_name, namespace_name);
    return;
  }
  char* json = fileio_read_all(file);
  fclose(file);

  jsmntok_t* jsmn = fileio_read_json(json);

  struct end_tile_com_ent com = {};
  if (end_tile_com_ent_fillout(mod_name, namespace_name, file_name, jsmn, json, &com) != 0) {
    free(json);
    free(jsmn);
    return;
  }

  free(json);
  free(jsmn);

  if (registry_add(end_regman_get_tile_com(), &com) == NULL) {
    log_error("Tile component %s:%s:%s already registered", mod_name, namespace_name, com.id);
    end_tile_com_ent_cleanup(&com);
    return;
  }

  log_info("Loading tile component %s:%s:%s", mod_name, namespace_name, com.id);
}

struct end_tile_com_ent* end_tile_com_ent_get(const char* ns, const char* id) {
  return registry_ktov(end_regman_get_tile_com(), &(struct end_tile_com_ent){.id = (char*)id, .namespace = ns});
}

int end_tile_com_ent_cmp(const struct end_tile_com_ent* a, const struct end_tile_com_ent* b) {
  int ns = registry_strcmp(a->namespace, b->namespace);
  if (ns != 0) return ns;
  return registry_strcmp(a->id, b->id);
}

void end_tile_com_ent_cleanup(struct end_tile_com_ent* elem) {
  free(elem->id);
}
