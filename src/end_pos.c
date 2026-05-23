#include "end_pos.h"

#include <jsmn_iterator.h>
#include <stdlib.h>
#include <string.h>

#include "end_api.h"

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
      char* str = jsmn_iterator_get_string_heap(json, iter.val);
      sys_pos->x = atoi(str);
      free(str);
    } else if (strcmp(iter.key, "y") == 0) {
      char* str = jsmn_iterator_get_string_heap(json, iter.val);
      sys_pos->y = atoi(str);
      free(str);
    } else if (strcmp(iter.key, "z") == 0) {
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
      char* str = jsmn_iterator_get_string_heap(json, iter.val);
      body_pos->x = strtoul(str, NULL, 10);
      free(str);
    } else if (strcmp(iter.key, "y") == 0) {
      char* str = jsmn_iterator_get_string_heap(json, iter.val);
      body_pos->y = strtoul(str, NULL, 10);
      free(str);
    } else if (strcmp(iter.key, "z") == 0) {
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

  pos->x = 0;
  pos->y = 0;

  struct jsmn_iterator iter;
  jsmn_iterator_init(&iter, jsmn, json);

  while (jsmn_iterator_next(&iter)) {
    if (strcmp(iter.key, "x") == 0) {
      char* str = jsmn_iterator_get_string_heap(json, iter.val);
      pos->x = atoi(str);
      free(str);
    } else if (strcmp(iter.key, "y") == 0) {
      char* str = jsmn_iterator_get_string_heap(json, iter.val);
      pos->y = atoi(str);
      free(str);
    } else {
      log_error("Unknown object %s in end_pos in file %s from %s:%s",
               iter.key, file_name, mod_name, namespace_name);
      error++;
    }
  }
  return error;
}

int end_pos_get_z(const struct end_pos* pos) { return pos->x + pos->y; }
