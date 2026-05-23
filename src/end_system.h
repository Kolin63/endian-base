#ifndef ENDIAN_BASE_END_SYSTEM_H_
#define ENDIAN_BASE_END_SYSTEM_H_

#include <registry.h>

#include "end_pos.h"

struct end_system {
  char* id;
  char* name;
  char* desc;
  struct registry body_ids;
  struct end_sys_pos pos;
};

void end_system_load(const char* system_path, const char* namespace_name,
                     const char* mod_name, const char* file_name);
struct end_system* end_system_get(const char* id);
int end_system_cmp(const struct end_system* a, const struct end_system* b);
void end_system_cleanup(struct end_system* elem);

struct end_system_body_id_entry {
  const char* id;
};

int end_system_body_id_entry_cmp(const struct end_system_body_id_entry* a, const struct end_system_body_id_entry* b);

#endif
