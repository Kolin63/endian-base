#include "end_pos.h"

#include <concord/jsmn.h>
#include <jsmn_iterator.h>
#include <json_macros.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "end_api.h"
#include "end_body.h"
#include "end_system.h"

int end_sys_pos_fillout(const char* namespace_name, const char* mod_name,
                        const char* file_name, const jsmntok_t* jsmn,
                        const char* json, struct end_sys_pos* sys_pos) {
  int error = 0;

  sys_pos->x = 0;
  sys_pos->y = 0;
  sys_pos->z = 0;

  struct jsmn_iterator iter;
  jsmn_iterator_init(&iter, jsmn, json);

  while (jsmn_iterator_next(&iter)) {
    if (strcmp(iter.key, "x") == 0) {
      END_JSON_CHECK_NUMBER(iter);
      char* str = jsmn_iterator_get_string_heap(json, iter.val);
      sys_pos->x = atoi(str);
      free(str);
    } else if (strcmp(iter.key, "y") == 0) {
      END_JSON_CHECK_NUMBER(iter);
      char* str = jsmn_iterator_get_string_heap(json, iter.val);
      sys_pos->y = atoi(str);
      free(str);
    } else if (strcmp(iter.key, "z") == 0) {
      END_JSON_CHECK_NUMBER(iter);
      char* str = jsmn_iterator_get_string_heap(json, iter.val);
      sys_pos->z = atoi(str);
      free(str);
    } else {
      log_error("Unknown object %s in end_sys_pos in file %s from %s:%s",
                iter.key, file_name, mod_name, namespace_name);
      error++;
    }
  }
  return error;
}

int end_body_pos_fillout(const char* namespace_name, const char* mod_name,
                         const char* file_name, const jsmntok_t* jsmn,
                         const char* json, struct end_body_pos* body_pos) {
  int error = 0;

  body_pos->x = 0;
  body_pos->y = 0;
  body_pos->z = 0;

  struct jsmn_iterator iter;
  jsmn_iterator_init(&iter, jsmn, json);

  while (jsmn_iterator_next(&iter)) {
    if (strcmp(iter.key, "x") == 0) {
      END_JSON_CHECK_NUMBER(iter);
      char* str = jsmn_iterator_get_string_heap(json, iter.val);
      body_pos->x = strtoul(str, NULL, 10);
      free(str);
    } else if (strcmp(iter.key, "y") == 0) {
      END_JSON_CHECK_NUMBER(iter);
      char* str = jsmn_iterator_get_string_heap(json, iter.val);
      body_pos->y = strtoul(str, NULL, 10);
      free(str);
    } else if (strcmp(iter.key, "z") == 0) {
      END_JSON_CHECK_NUMBER(iter);
      char* str = jsmn_iterator_get_string_heap(json, iter.val);
      body_pos->z = strtoul(str, NULL, 10);
      free(str);
    } else {
      log_error("Unknown object %s in end_body_pos in file %s from %s:%s",
                iter.key, file_name, mod_name, namespace_name);
      error++;
    }
  }
  return error;
}

int end_pos_fillout(const char* namespace_name, const char* mod_name,
                    const char* file_name, const jsmntok_t* jsmn,
                    const char* json, struct end_pos* pos) {
  int error = 0;

  pos->body = NULL;
  pos->system = NULL;
  pos->x = 0;
  pos->y = 0;

  struct jsmn_iterator iter;
  jsmn_iterator_init(&iter, jsmn, json);

  while (jsmn_iterator_next(&iter)) {
    if (strcmp(iter.key, "body") == 0) {
      // body can be null
      if (iter.val->type == JSMN_PRIMITIVE) {
        char buf[2];
        jsmn_iterator_get_string(buf, sizeof(buf), json, iter.val);
        if (buf[0] == 'n') continue;
      }
      END_JSON_CHECK_STRING(iter);
      // only for cmp purposes, don't set pos->body to this
      char* ns = jsmn_iterator_get_string_heap(json, iter.val);
      char* colon = ns;
      while (*colon != ':' && *colon != '\0') colon++;
      if (*colon == '\0') {
        log_error("Body in pos %s:%s:%s not formatted in namespace:bodyname (got %s)",
                  mod_name, namespace_name, file_name, ns);
        error++;
        free(ns);
        continue;
      }
      *colon = '\0';
      const char* id = colon + 1;

      const struct end_body* body = end_body_get(ns, id);
      if (body == NULL) {
        log_error("Body %s:%s is not registered in file %s from %s:%s", ns, id, file_name, mod_name, namespace_name);
        error++;
        free(ns);
        continue;
      }
      free(ns);
      pos->body_ns = body->namespace;
      pos->body = body->id;
    } else if (strcmp(iter.key, "system") == 0) {
      // system can be null
      if (iter.val->type == JSMN_PRIMITIVE) {
        char buf[2];
        jsmn_iterator_get_string(buf, sizeof(buf), json, iter.val);
        if (buf[0] == 'n') continue;
      }
      END_JSON_CHECK_STRING(iter);
      // only for cmp purposes, don't set pos->system to this
      char* ns = jsmn_iterator_get_string_heap(json, iter.val);
      char* colon = ns;
      while (*colon != ':' && *colon != '\0') colon++;
      if (*colon == '\0') {
        log_error("System in pos %s:%s:%s not formatted in namespace:sysname (got %s)",
                  mod_name, namespace_name, file_name, ns);
        error++;
        free(ns);
        continue;
      }
      *colon = '\0';
      const char* id = colon + 1;

      const struct end_system* sys = end_system_get(ns, id);
      if (sys == NULL) {
        log_error("System %s:%s is not registered in file %s from %s:%s", ns, id, file_name, mod_name, namespace_name);
        error++;
        free(ns);
        continue;
      }
      free(ns);
      pos->system = sys->id;
      pos->system_ns = sys->namespace;
    } else if (strcmp(iter.key, "x") == 0) {
      END_JSON_CHECK_NUMBER(iter);
      char* str = jsmn_iterator_get_string_heap(json, iter.val);
      pos->x = atoi(str);
      free(str);
    } else if (strcmp(iter.key, "y") == 0) {
      END_JSON_CHECK_NUMBER(iter);
      char* str = jsmn_iterator_get_string_heap(json, iter.val);
      pos->y = atoi(str);
      free(str);
    } else {
      log_error("Unknown object %s in end_pos in file %s from %s:%s",
                iter.key, file_name, mod_name, namespace_name);
      error++;
    }
  }

  // check that the body is in the system
  if (pos->body != NULL && pos->system != NULL) {
    const struct end_system* sys = end_system_get(pos->system_ns, pos->system);
    const struct end_system_body_id_entry key = {.id = pos->body, .namespace = pos->body_ns};
    const int i = registry_ktoi(&(sys->body_ids), &key);
    if (i < 0) {
      log_error("Body %s:%s is not in system %s:%s in file %s from %s:%s",
                pos->body_ns, pos->body, pos->system_ns, pos->system, file_name, mod_name, namespace_name);
      error++;
    }
  }

  return error;
}

void end_pos_human_readable(char* buf, size_t size, struct end_pos pos) {
  const int x = pos.x;
  const int y = pos.y;
  const int z = pos.z;
  const char* body = pos.body;
  const char* system = pos.system;

  if (body != NULL && system != NULL) {
    snprintf(buf, size, "(%i, %i) on %s, %s", x, y, body, system);
  } else if (body == NULL && system != NULL) {
    snprintf(buf, size, "(%i, %i, %i) in %s", x, y, z, system);
  } else if (body != NULL && system == NULL) {
    snprintf(buf, size, "(%i, %i, %i) on %s, interstellar", x, y, z, body);
  } else if (body == NULL && system == NULL) {
    snprintf(buf, size, "(%i, %i, %i) interstellar", x, y, z);
  }
}
