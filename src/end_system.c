#include "end_system.h"

#include <save.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "end_api.h"
#include "end_body.h"
#include "end_pos.h"
#include "end_regman.h"
#include "json_macros.h"
#include "registry.h"
#include "str_cat_arr.h"

int end_system_fillout(const char* namespace_name, const char* mod_name,
                       const char* file_name, const jsmntok_t* jsmn,
                       const char* json, struct end_system* sys) {
  int error = 0;

  sys->fid.ns = namespace_name;

  struct jsmn_iterator iter;
  jsmn_iterator_init(&iter, jsmn, json);

  while (jsmn_iterator_next(&iter)) {
    if (strcmp(iter.key, "id") == 0) {
      END_JSON_CHECK_STRING(iter);
      sys->fid.id = jsmn_iterator_get_string_heap(json, iter.val);
    } else if (strcmp(iter.key, "name") == 0) {
      END_JSON_CHECK_STRING(iter);
      sys->name = jsmn_iterator_get_string_heap(json, iter.val);
    } else if (strcmp(iter.key, "desc") == 0) {
      END_JSON_CHECK_STRING(iter);
      sys->desc = jsmn_iterator_get_string_heap(json, iter.val);
    } else if (strcmp(iter.key, "pos") == 0) {
      end_sys_pos_fillout(namespace_name, mod_name, file_name, iter.val, json, &(sys->pos));
    } else {
      log_error("Unknown object %s in end_system in file %s from %s:%s",
                iter.key, file_name, mod_name, namespace_name);
      error++;
    }
  }

  return error;
}

void end_system_load(const char* system_path, const char* namespace_name,
                     const char* mod_name, const char* file_name) {
  if (strcmp(file_name, "template") == 0) return;

  // +1 to account for trailing slash that will be added
  const size_t system_path_len = strlen(system_path) + 1;
  char* path_buf = malloc(system_path_len + 16);
  strcpy(path_buf, system_path);
  path_buf[system_path_len - 1] = '/';

  // pointer to the spot that can be written to in order to make a path
  // relative to system root. eg) to write "info.json"
  char* const sys_rel_path = path_buf + system_path_len;

  // fill out system info
  strcpy(sys_rel_path, "info.json");
  FILE* file = fopen(path_buf, "r");
  if (file == NULL) {
    log_error("Could not open 'info.json' at %s from %s:%s", path_buf, mod_name, namespace_name);
    return;
  }
  char* json = fileio_read_all(file);
  fclose(file);

  jsmntok_t* jsmn = fileio_read_json(json);

  struct end_system sys = {};
  if (end_system_fillout(namespace_name, mod_name, file_name, jsmn, json, &sys) != 0) {
    free(json);
    free(jsmn);
    return;
  }

  free(json);
  free(jsmn);

  registry_init(&(sys.body_ids), sizeof(struct end_system_body_id_entry), (void*)end_system_body_id_entry_cmp, NULL);

  log_info("Loading bodies from system %s:%s:%s", mod_name, namespace_name, sys.fid.id);

  strcpy(sys_rel_path, "bodies");
  dir_load(path_buf, end_body_load(&sys, file_path, namespace_name, mod_name, file_name));

  free(path_buf);

  if (registry_add(end_regman_get_system(), &sys) == NULL) {
    log_error("System %s:%s:%s already registered", mod_name, namespace_name, sys.fid.id);
    end_system_cleanup(&sys);
    return;
  }

  log_info("Done loading system %s:%s:%s", mod_name, namespace_name, sys.fid.id);
}

struct end_system* end_system_get(const struct fid* fid) {
  return registry_ktov(end_regman_get_system(), &(struct end_system){.fid = *fid});
}

int end_system_cmp(const struct end_system* a, const struct end_system* b) {
  int ns = registry_strcmp(a->fid.ns, b->fid.ns);
  if (ns != 0) return ns;
  return registry_strcmp(a->fid.id, b->fid.id);
}

void end_system_cleanup(struct end_system* elem) {
  free((char*)elem->fid.id);
  free(elem->name);
  free(elem->desc);
  registry_cleanup(&(elem->body_ids));
}

void end_system_load_save(const char* file_path, const char* namespace_name, const char* file_name) {
}

void end_system_save(const struct end_system* sys) {
  const char* arr[] = {
      STR({}),
  };

  char* str = str_cat_arr(arr, sizeof(arr));

  save_write(sys->fid.ns, "systems", sys->fid.id, "json", str);

  free(str);
}

void end_system_save_all() {
  const struct registry* reg = end_regman_get_system();
  for (int i = 0; i < reg->length; i++) {
    const struct end_system* sys = registry_itov(reg, i);
    log_info("Saving system %s:%s", sys->fid.ns, sys->fid.id);
    end_system_save(sys);
  }
}

int end_system_body_id_entry_cmp(const struct end_system_body_id_entry* a, const struct end_system_body_id_entry* b) {
  return a->id - b->id;
}
