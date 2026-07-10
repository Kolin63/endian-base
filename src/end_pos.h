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

struct end_pos_rich {
  const struct fid* sys;
  const struct fid* body;
  unsigned int x;
  unsigned int y;
  unsigned int z;
};

struct end_pos {
  const struct fid* sys;
  const struct fid* body;
  unsigned int i;
};

int end_sys_pos_fillout(const char* mod_name, const char* namespace_name,
                        const char* file_name, const jsmntok_t* jsmn,
                        const char* json, struct end_sys_pos* sys_pos);
char* end_sys_pos_to_json(const struct end_sys_pos* sys_pos);

int end_body_pos_fillout(const char* mod_name, const char* namespace_name,
                         const char* file_name, const jsmntok_t* jsmn,
                         const char* json, struct end_body_pos* body_pos);
char* end_body_pos_to_json(const struct end_body_pos* body_pos);

int end_pos_fillout(const char* mod_name, const char* namespace_name,
                    const char* file_name, const jsmntok_t* jsmn,
                    const char* json, struct end_pos* pos);
char* end_pos_to_json(const struct end_pos* pos);

// buf should be 128 characters long
void end_pos_human_readable(char* buf, size_t size, struct end_pos pos);

struct end_pos_rich end_pos_get_rich(const struct end_pos* pos);
struct end_pos end_pos_rich_get_pos(const struct end_pos_rich* pos);

#endif
