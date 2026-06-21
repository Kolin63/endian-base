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

struct end_pos {
  const struct fid* sys;
  const struct fid* body;
  int x;
  int y;
  int z;
};

int end_sys_pos_fillout(const char* mod_name, const char* namespace_name,
                        const char* file_name, const jsmntok_t* jsmn,
                        const char* json, struct end_sys_pos* sys_pos);

int end_body_pos_fillout(const char* mod_name, const char* namespace_name,
                         const char* file_name, const jsmntok_t* jsmn,
                         const char* json, struct end_body_pos* body_pos);

int end_pos_fillout(const char* mod_name, const char* namespace_name,
                    const char* file_name, const jsmntok_t* jsmn,
                    const char* json, struct end_pos* pos);

// buf should be 128 characters long
void end_pos_human_readable(char* buf, size_t size, struct end_pos pos);

#endif
