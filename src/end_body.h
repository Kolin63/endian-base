#ifndef ENDIAN_BASE_END_BODY_TYPE_H_
#define ENDIAN_BASE_END_BODY_TYPE_H_

#include <time.h>

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
  const char* namespace;
  char* name;
  char* desc;
  enum end_body_type type;

  // can be NULL
  const char* primary;
  char* primary_namespace;

  const char* system;
  const char* system_namespace;

  spaceint_t mass;

  unsigned int radius;

  // in seconds
  time_t orbital_period;

  unsigned int face_width;
  unsigned int total_tiles;

  // can be 0
  unsigned long semimajoraxis;

  struct end_body_pos pos;
};

void end_body_load(struct end_system* system,
                   const char* body_path, const char* namespace_name,
                   const char* mod_name, const char* file_name);
struct end_body* end_body_get(const char* ns, const char* id);
int end_body_cmp(const struct end_body* a, const struct end_body* b);
void end_body_cleanup(struct end_body* elem);

// returns orbital period in seconds
time_t calc_orbital_period(unsigned long semimajoraxis, spaceint_t larger_mass);

// returns the width of one face of body in tiles
unsigned int calc_face_width(unsigned int radius);

// assumes that every referenced body is registered
// returns 0 if ok
// calculates orbital periods for all registered bodies
int end_body_post_load_fillout();

#endif
