#ifndef ENDIAN_BASE_POS_H_
#define ENDIAN_BASE_POS_H_

#define JSMN_HEADER
#include <concord/jsmn.h>

struct end_sys_pos {
  int x;
  int y;
  int z;
};

// in kilometers
struct end_body_pos {
  unsigned long x;
  unsigned long y;
  unsigned long z;
};

// hex grid cube coordinates
struct end_pos {
  const char* body;
  int x;
  int y;
};

int end_sys_pos_fillout(const char* namespace_name, const char* mod_name,
                        const char* file_name, const jsmntok_t* jsmn,
                        const char* json, struct end_sys_pos* sys_pos);

int end_body_pos_fillout(const char* namespace_name, const char* mod_name,
                         const char* file_name, const jsmntok_t* jsmn,
                         const char* json, struct end_body_pos* body_pos);

int end_pos_fillout(const char* namespace_name, const char* mod_name,
                    const char* file_name, const jsmntok_t* jsmn,
                    const char* json, struct end_pos* pos);

int end_pos_get_z(const struct end_pos* pos);

#endif
