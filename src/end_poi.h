#ifndef ENDIAN_BASE_END_POI_H_
#define ENDIAN_BASE_END_POI_H_

#include <registry.h>

#include "end_coms.h"
#include "ref/end_poi_types.ref.h"

// for each type: END_POI_%N_%D
enum end_poi_type_id;

struct end_poi {
  enum end_poi_type_id type;
  void* coms;
};

void end_poi_reg_init();
void end_poi_reg_cleanup();
const struct registry* end_poi_reg_get();

struct end_poi* end_poi_add(enum end_poi_type_id type, void* coms);

void* end_poi_coms_init(enum end_poi_type_id type);  // allocates block and zero initializes, then returns block
void end_poi_coms_cleanup(enum end_poi_type_id type, void* coms);
void* end_poi_coms_get_com(enum end_poi_type_id type, void* coms, enum end_com_id com_id);
void* end_poi_get_com(struct end_poi* poi, enum end_com_id com_id);

enum end_poi_type_id end_poi_type_id_get(const char* id_str);
bool end_poi_type_verify(enum end_poi_type_id type);
const char* end_poi_type_to_str(enum end_poi_type_id type);

#endif
