#include "end_body.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "end_api.h"
#include "end_regman.h"
#include "json_macros.h"
#include "registry.h"
#include "spaceint.h"

int end_body_fillout(const char* namespace_name, const char* mod_name,
                     const char* file_name, const jsmntok_t* jsmn,
                     const char* json, struct end_body* body) {
  int error = 0;

  body->primary = NULL;
  body->semimajoraxis = 0;

  struct jsmn_iterator iter;
  jsmn_iterator_init(&iter, jsmn, json);

  while (jsmn_iterator_next(&iter)) {
    if (strcmp(iter.key, "id") == 0) {
      END_JSON_CHECK_STRING(iter);
      body->id = jsmn_iterator_get_string_heap(json, iter.val);
    } else if (strcmp(iter.key, "name") == 0) {
      END_JSON_CHECK_STRING(iter);
      body->name = jsmn_iterator_get_string_heap(json, iter.val);
    } else if (strcmp(iter.key, "desc") == 0) {
      END_JSON_CHECK_STRING(iter);
      body->desc = jsmn_iterator_get_string_heap(json, iter.val);
    } else if (strcmp(iter.key, "type") == 0) {
      END_JSON_CHECK_STRING(iter);
      char type[64];
      jsmn_iterator_get_string(type, sizeof(type), json, iter.val);
      if (strcmp(type, "STAR") == 0) body->type = END_BODY_STAR;
      else if (strcmp(type, "TERRESTRIAL") == 0) body->type = END_BODY_TERRESTRIAL;
      else if (strcmp(type, "GAS_GIANT") == 0) body->type = END_BODY_GAS_GIANT;
      else if (strcmp(type, "ICE_GIANT") == 0) body->type = END_BODY_ICE_GIANT;
      else if (strcmp(type, "MOON") == 0) body->type = END_BODY_MOON;
    } else if (strcmp(iter.key, "primary") == 0) {
      END_JSON_CHECK_STRING(iter);
      body->primary = jsmn_iterator_get_string_heap(json, iter.val);
    } else if (strcmp(iter.key, "mass") == 0) {
      END_JSON_CHECK_NUMBER(iter);
      char scinot[128];
      jsmn_iterator_get_string(scinot, sizeof(scinot), json, iter.val);
      body->mass = scinot_to_spaceint(scinot);
    } else if (strcmp(iter.key, "radius") == 0) {
      END_JSON_CHECK_NUMBER(iter);
      char str[128];
      jsmn_iterator_get_string(str, sizeof(str), json, iter.val);
      body->radius = strtoul(str, NULL, 10);
    } else if (strcmp(iter.key, "semimajoraxis") == 0) {
      END_JSON_CHECK_NUMBER(iter);
      char str[128];
      jsmn_iterator_get_string(str, sizeof(str), json, iter.val);
      body->semimajoraxis = strtoul(str, NULL, 10);
    } else if (strcmp(iter.key, "pos") == 0) {
      end_body_pos_fillout(namespace_name, mod_name, file_name, iter.val, json, &(body->pos));
    } else {
      log_error("Unknown object %s in end_system in file %s from %s:%s",
                iter.key, file_name, mod_name, namespace_name);
      error++;
    }
  }

  return error;
}

void end_body_load(struct end_system* system,
                   const char* body_path, const char* namespace_name,
                   const char* mod_name, const char* file_name) {
  if (strcmp(file_name, "template") == 0) return;

  FILE* file = fopen(body_path, "r");
  if (file == NULL) {
    log_error("Could not open 'info.json' at %s from %s:%s", body_path, namespace_name, mod_name);
    return;
  }
  char* json = fileio_read_all(file);
  fclose(file);

  jsmntok_t* jsmn = fileio_read_json(json);

  struct end_body body = {};
  if (end_body_fillout(namespace_name, mod_name, file_name, jsmn, json, &body) != 0) {
    free(json);
    free(jsmn);
    return;
  }

  free(json);
  free(jsmn);

  body.system = system->id;

  if (registry_add(end_regman_get_body(), &body) == NULL) {
    log_error("Body %s:%s:%s already registered", namespace_name, mod_name, body.id);
    end_body_cleanup(&body);
    return;
  }

  registry_add(&(system->body_ids), &(struct end_system_body_id_entry){.id = body.id});

  log_info("Loading body %s:%s:%s", namespace_name, mod_name, body.id);
}

struct end_body* end_body_get(char* id) {
  return registry_ktov(end_regman_get_body(), &(struct end_body){id = id});
}

int end_body_cmp(const struct end_body* a, const struct end_body* b) {
  return registry_strcmp(a->id, b->id);
}

void end_body_cleanup(struct end_body* elem) {
  free(elem->id);
  free(elem->name);
  free(elem->desc);
  if (elem->primary != NULL) free(elem->primary);
}
