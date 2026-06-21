#include "end_tile.h"

#include <stdlib.h>
#include <string.h>

#include "end_api.h"
#include "end_regman.h"
#include "end_tile_com.h"
#include "fid.h"
#include "jsmn_iterator.h"
#include "json_macros.h"

int end_tile_fillout(const char* mod_name, const char* namespace_name,
                     const char* file_name, const jsmntok_t* jsmn,
                     const char* json, struct end_tile* tile) {
  int error = 0;

  tile->id = -1;
  registry_init(&tile->coms, sizeof(struct end_tile_com), (void*)end_tile_com_cmp, (void*)end_tile_com_cleanup);

  struct jsmn_iterator iter;
  jsmn_iterator_init(&iter, jsmn, json);

  /*
      {
        "id": "foo:bar",
        "coms": {
          "baz:bop": {
            "flip": 5,
            "flop": 10
          }
        }
      }
   */
  while (jsmn_iterator_next(&iter)) {
    if (strcmp(iter.key, "id") == 0) {
      END_JSON_CHECK_STRING(iter);
      char* str = jsmn_iterator_get_string_heap(json, iter.val);
      struct fid fid = fid_split(str);
      if (fid.ns == NULL) {
        log_error("In tilemap tile %s:%s:%s, id is not ns:id (got %s)",
                  mod_name, namespace_name, file_name, str);
        error++;
        free(str);
        continue;
      }
      const struct end_tile_ent* ent = end_tile_ent_get(&fid);
      if (ent == NULL) {
        log_error("In tilemap tile %s:%s:%s, tile %s does not exist",
                  mod_name, namespace_name, file_name, str);
        error++;
        free(str);
        continue;
      }
      tile->id = end_tile_ent_get_int_id(ent);
      free(str);
    } else if (strcmp(iter.key, "coms") == 0) {
      END_JSON_CHECK_OBJECT(iter);
      struct jsmn_iterator coms_iter;
      jsmn_iterator_init(&coms_iter, iter.val, json);

      while (jsmn_iterator_next(&coms_iter)) {
        struct end_tile_com com = {};
        int this_error = end_tile_com_fillout(mod_name, namespace_name, file_name,
                                              coms_iter.val, json, coms_iter.key, &com);
        if (this_error != 0) {
          log_error("In tilemap tile %s:%s:%s, could not parse component %s",
                    mod_name, namespace_name, file_name, coms_iter.key);
          error += this_error;
          end_tile_com_cleanup(&com);
          continue;
        }
        if (registry_add(&tile->coms, &com) == NULL) {
          log_error("In tilemap tile %s:%s:%s, component %s already exists",
                    mod_name, namespace_name, file_name, coms_iter.key);
          error++;
          end_tile_com_cleanup(&com);
          continue;
        }
      }
    }
  }

  if (tile->id == -1) {
    log_error("In tilemap tile %s:%s:%s, tile id was not set",
              mod_name, namespace_name, file_name);
    error++;
  }

  return error;
}

void end_tile_cleanup(struct end_tile* elem) {
  registry_cleanup(&elem->coms);
}

const struct end_tile_ent* end_tile_get_ent(const struct end_tile* tile) {
  return registry_itov_safe(end_regman_get_tile(), tile->id);
}

int end_tile_ent_coms_fillout(const char* mod_name, const char* namespace_name,
                              const char* file_name, const jsmntok_t* jsmn,
                              const char* json, struct registry* reg) {
  int error = 0;

  struct jsmn_iterator iter;
  jsmn_iterator_init(&iter, jsmn, json);

  while (jsmn_iterator_next(&iter)) {
    END_JSON_CHECK_STRING(iter);
    char* str = jsmn_iterator_get_string_heap(json, iter.val);
    struct fid fid = fid_split(str);
    if (fid.ns == NULL) {
      log_error("Tile component ID from %s:%s:%s is not formatted in namespace:com (got %s)",
                mod_name, namespace_name, file_name, str);
      error++;
      free(str);
      continue;
    }
    const struct end_tile_com_ent* ent = end_tile_com_ent_get(&fid);
    if (ent == NULL) {
      log_error("Tile compenent %s:%s does not exist (from %s:%s:%s)",
                fid.ns, fid.id, mod_name, namespace_name, file_name);
      error++;
      free(str);
      continue;
    }

    int i = registry_vtoi(end_regman_get_tile_com(), ent);
    struct end_tile_com com = {.data = NULL, .id = i};
    if (registry_add(reg, &com) == NULL) {
      log_error("Tile %s:%s:%s already has component %s:%s",
                mod_name, namespace_name, file_name, fid.ns, fid.id);
      error++;
      free(str);
      continue;
    }

    free(str);
  }

  return error;
}

int end_tile_ent_fillout(const char* mod_name, const char* namespace_name,
                         const char* file_name, const jsmntok_t* jsmn,
                         const char* json, struct end_tile_ent* tile) {
  int error = 0;

  tile->fid.ns = namespace_name;

  registry_init(&(tile->coms), sizeof(struct end_tile_com), (void*)end_tile_com_cmp, (void*)end_tile_com_cleanup);

  struct jsmn_iterator iter;
  jsmn_iterator_init(&iter, jsmn, json);

  while (jsmn_iterator_next(&iter)) {
    if (strcmp(iter.key, "id") == 0) {
      END_JSON_CHECK_STRING(iter);
      tile->fid.id = jsmn_iterator_get_string_heap(json, iter.val);
    } else if (strcmp(iter.key, "name") == 0) {
      END_JSON_CHECK_STRING(iter);
      tile->name = jsmn_iterator_get_string_heap(json, iter.val);
    } else if (strcmp(iter.key, "desc") == 0) {
      END_JSON_CHECK_STRING(iter);
      tile->desc = jsmn_iterator_get_string_heap(json, iter.val);
    } else if (strcmp(iter.key, "icon") == 0) {
      END_JSON_CHECK_STRING(iter);
      char buf[2];
      jsmn_iterator_get_string(buf, sizeof(buf), json, iter.val);
      tile->icon = buf[0];
    } else if (strcmp(iter.key, "coms") == 0) {
      END_JSON_CHECK_ARRAY(iter);
      error += end_tile_ent_coms_fillout(mod_name, namespace_name, file_name, iter.val, json, &(tile->coms));
    }
  }

  return error;
}

void end_tile_ent_load(const char* file_path, const char* mod_name,
                       const char* namespace_name, const char* file_name) {
  if (strcmp(file_name, "template.json") == 0) return;

  FILE* file = fopen(file_path, "r");
  if (file == NULL) {
    log_error("Could not open %s from %s:%s", file_path, mod_name, namespace_name);
    return;
  }
  char* json = fileio_read_all(file);
  fclose(file);

  jsmntok_t* jsmn = fileio_read_json(json);

  struct end_tile_ent tile = {};
  if (end_tile_ent_fillout(mod_name, namespace_name, file_name, jsmn, json, &tile) != 0) {
    free(json);
    free(jsmn);
    return;
  }

  free(json);
  free(jsmn);

  if (registry_add(end_regman_get_tile(), &tile) == NULL) {
    log_error("Tile %s:%s:%s already registered", mod_name, namespace_name, tile.fid.id);
    end_tile_ent_cleanup(&tile);
    return;
  }

  log_info("Loading tile %s:%s:%s", mod_name, namespace_name, tile.fid.id);
}

struct end_tile_ent* end_tile_ent_get(const struct fid* fid) {
  return registry_ktov(end_regman_get_tile(), &(struct end_tile_ent){.fid = *fid});
}

int end_tile_ent_get_int_id(const struct end_tile_ent* ent) {
  return registry_vtoi(end_regman_get_tile(), ent);
}

int end_tile_ent_cmp(const struct end_tile_ent* a, const struct end_tile_ent* b) {
  int ns = registry_strcmp(a->fid.ns, b->fid.ns);
  if (ns != 0) return ns;
  return registry_strcmp(a->fid.id, b->fid.id);
}

void end_tile_ent_cleanup(struct end_tile_ent* elem) {
  free((char*)elem->fid.id);
  free(elem->name);
  free(elem->desc);
  registry_cleanup(&elem->coms);
}
