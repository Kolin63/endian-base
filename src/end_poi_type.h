#ifndef ENDIAN_BASE_POI_TYPE_H_
#define ENDIAN_BASE_POI_TYPE_H_

struct end_poi_type {
  const char* name;
};

enum end_poi_type_id;

static struct end_poi_type end_poi_types[];

enum end_poi_type_id end_poi_type_id_get(const char* id_str);

#include "ref/end_poi_types.ref.h"

#endif
