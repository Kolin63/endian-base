#include "end_body.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "end_api.h"
#include "end_regman.h"
#include "end_system.h"
#include "end_tilemap.h"
#include "fid.h"
#include "json_macros.h"
#include "registry.h"
#include "spaceint.h"

int end_body_fillout(const char* namespace_name, const char* mod_name,
                     const char* file_name, const jsmntok_t* jsmn,
                     const char* json, struct end_body* body) {
  int error = 0;

  body->fid.ns = namespace_name;
  body->prim_fid = (struct fid){};
  body->semimajoraxis = 0;
  body->tilemap = (struct end_tilemap){.face_width = 0, .total_tiles = 0, .tiles = NULL};

  struct jsmn_iterator iter;
  jsmn_iterator_init(&iter, jsmn, json);

  while (jsmn_iterator_next(&iter)) {
    if (strcmp(iter.key, "id") == 0) {
      END_JSON_CHECK_STRING(iter);
      char* id = jsmn_iterator_get_string_heap(json, iter.val);
      body->fid.id = id;
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
      char* str = jsmn_iterator_get_string_heap(json, iter.val);
      struct fid fid = fid_split(str);

      if (fid.ns == NULL) {
        log_error("Primary body from %s:%s:%s is not formatted in namespace:bodyname (got %s)",
                  mod_name, namespace_name, file_name, str);
        error++;
        free(str);
        continue;
      }

      body->prim_fid = fid;
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
      body->tilemap.face_width = body->radius * 0.00565313480124;
      if (body->tilemap.face_width == 0) body->tilemap.face_width = 1;
    } else if (strcmp(iter.key, "semimajoraxis") == 0) {
      END_JSON_CHECK_NUMBER(iter);
      char str[128];
      jsmn_iterator_get_string(str, sizeof(str), json, iter.val);
      body->semimajoraxis = strtoul(str, NULL, 10);
    } else if (strcmp(iter.key, "pos") == 0) {
      error += end_body_pos_fillout(namespace_name, mod_name, file_name, iter.val, json, &(body->pos));
    } else if (strcmp(iter.key, "tiles") == 0) {
      error += end_tilemap_fillout(namespace_name, mod_name, file_name, iter.val, json, &(body->tilemap));
    } else {
      log_error("Unknown object %s in end_body in file %s from %s:%s",
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
    log_error("Could not open %s from %s:%s", body_path, mod_name, namespace_name);
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

  body.sys_fid = system->fid;

  if (registry_add(end_regman_get_body(), &body) == NULL) {
    log_error("Body %s:%s:%s already registered", mod_name, namespace_name, file_name);
    end_body_cleanup(&body);
    return;
  }

  log_info("Loading body %s:%s:%s", mod_name, namespace_name, file_name);
}

struct end_body* end_body_get(const struct fid* fid) {
  return registry_ktov(end_regman_get_body(), &(struct end_body){.fid = *fid});
}

int end_body_cmp(const struct end_body* a, const struct end_body* b) {
  int ns = registry_strcmp(a->fid.ns, b->fid.ns);
  if (ns != 0) return ns;
  return registry_strcmp(a->fid.id, b->fid.id);
}

void end_body_cleanup(struct end_body* elem) {
  free((char*)elem->fid.id);
  free(elem->name);
  free(elem->desc);
}

time_t calc_orbital_period(unsigned long semimajoraxis, spaceint_t larger_mass) {
  // https://www.desmos.com/calculator/wobxspxb65
  const spaceint_t a = semimajoraxis;
  const spaceint_t M = larger_mass;
  return spaceint_sqrt((_BitInt(256))(4000000000 * 9.86960440109 * a * a * a) / ((66743 * M) / (1000000000000000)));
}

unsigned int calc_face_width(unsigned int radius) {
  // https://www.desmos.com/calculator/retkabho7a
  // radius <= 176 would equate to 0, but we don't want bodies with no body
  if (radius <= 176) return 1;
  return radius * 0.00565313480124;
}

int end_body_post_load_fillout() {
  int error = 0;
  struct registry* reg = end_regman_get_body();

  for (int i = 0; i < reg->length; i++) {
    struct end_body* body = registry_itov(reg, i);
    struct end_system* sys = end_system_get(&body->sys_fid);

    // put integer id on system registry
    registry_add(&(sys->body_ids), &(struct end_system_body_id_entry){.id = i});

    // if the primary fid is NULL, the body intentionally has no primary
    if (body->prim_fid.id != NULL) {
      // get primary and check that it exists
      struct end_body* prim = end_body_get(&body->prim_fid);
      if (prim == NULL) {
        log_error("From body %s:%s, primary %s:%s does not exist",
                  body->fid.ns, body->fid.id, body->prim_fid.ns, body->prim_fid.id);
        error++;
        continue;
      }

      // replace the body's primary fid with a pointer to the primary's fid
      free((char*)body->prim_fid.ns);
      body->prim_fid = prim->fid;

      // calculate all orbital periods
      body->orbital_period = calc_orbital_period(body->semimajoraxis, prim->mass);
    }

    // calculate face width and total tiles
    body->tilemap.face_width = calc_face_width(body->radius);
    body->tilemap.total_tiles = (body->tilemap.face_width * body->tilemap.face_width) * 6;
  }
  return error;
}
