#ifndef ENDIAN_BASE_END_BODY_TYPE_H_
#define ENDIAN_BASE_END_BODY_TYPE_H_

#include "end_pos.h"
#include "end_system.h"
#include "spaceint.h"

enum end_body_type {
  END_BODY_STAR,
  END_BODY_TERRESTRIAL,
  END_BODY_GAS_GIANT,
  END_BODY_ICE_GIANT,
  END_BODY_MOON,
};

struct end_body {
  char* id;
  char* name;
  char* desc;
  enum end_body_type type;

  // can be NULL
  char* primary;

  const char* system;

  spaceint_t mass;

  // can be 0
  unsigned long semimajoraxis;

  struct end_body_pos pos;
};

void end_body_load(struct end_system* system,
                   const char* body_path, const char* namespace_name,
                   const char* mod_name, const char* file_name);
struct end_body* end_body_get(char* system, char* id);
int end_body_cmp(const struct end_body* a, const struct end_body* b);
void end_body_cleanup(struct end_body* elem);

#endif
