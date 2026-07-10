#include "end_tile_com_tag.h"

#include <stdlib.h>
#include <string.h>

#include "end_api.h"
#include "end_regman.h"
#include "json_macros.h"

struct end_tile_com_tag end_tile_com_tag_get(const struct fid* fid) {
  const struct end_tile_com_tag_ent* ent = end_tile_com_tag_ent_get(fid);
  if (ent == NULL) return (struct end_tile_com_tag){.id = -1};
  int i = registry_vtoi(end_regman_get_tile_com(), ent);
  return (struct end_tile_com_tag){.id = i};
}

int end_tile_com_tag_cmp(const struct end_tile_com_tag* a, const struct end_tile_com_tag* b) {
  return a->id - b->id;
}

void end_tile_com_tag_cleanup(struct end_tile_com_tag* elem) {}

int end_tile_com_tag_ent_fillout(const char* mod_name, const char* namespace_name,
                                 const char* file_name, const jsmntok_t* jsmn,
                                 const char* json, struct end_tile_com_tag_ent* tag) {
  int error = 0;

  tag->fid.ns = namespace_name;
  tag->fid.id = NULL;

  struct jsmn_iterator iter;
  jsmn_iterator_init(&iter, jsmn, json);

  while (jsmn_iterator_next(&iter)) {
    if (strcmp(iter.key, "id") == 0) {
      END_JSON_CHECK_STRING(iter);
      tag->fid.id = jsmn_iterator_get_string_heap(json, iter.val);
    } else {
      log_error("In tile com tag %s:%s:%s, unknown object %s",
                mod_name, namespace_name, file_name, iter.key);
      error++;
      return error;
    }
  }

  if (tag->fid.id == NULL) {
    log_error("In tile com tag %s:%s:%s, id must be specified",
              mod_name, namespace_name, file_name);
    error++;
    return error;
  }

  return error;
}

void end_tile_com_tag_ent_load(const char* file_path, const char* mod_name,
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

  struct end_tile_com_tag_ent tag = {};
  if (end_tile_com_tag_ent_fillout(mod_name, namespace_name, file_name, jsmn, json, &tag) != 0) {
    free(json);
    free(jsmn);
    return;
  }

  free(json);
  free(jsmn);

  if (registry_add(end_regman_get_tile_com_tag(), &tag) == NULL) {
    log_error("Tile component tag %s:%s:%s already registered", mod_name, namespace_name, tag.fid.id);
    end_tile_com_tag_ent_cleanup(&tag);
    return;
  }

  log_info("Loading tile component tag %s:%s:%s", mod_name, namespace_name, tag.fid.id);
}

struct end_tile_com_tag_ent* end_tile_com_tag_ent_get(const struct fid* fid) {
  return registry_ktov(end_regman_get_tile_com_tag(), &(struct end_tile_com_tag_ent){.fid = *fid});
}

int end_tile_com_tag_ent_cmp(const struct end_tile_com_tag_ent* a, const struct end_tile_com_tag_ent* b) {
  int ns = registry_strcmp(a->fid.ns, b->fid.ns);
  if (ns != 0) return ns;
  return registry_strcmp(a->fid.id, b->fid.id);
}

void end_tile_com_tag_ent_cleanup(struct end_tile_com_tag_ent* elem) {
  free((char*)elem->fid.id);
}
