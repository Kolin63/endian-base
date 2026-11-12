#ifndef ENDIAN_BASE_END_POI_H_
#define ENDIAN_BASE_END_POI_H_

#include "end_poi_type.h"

struct end_poi {
  enum end_poi_type_id id;
  struct end_coms coms;
};

#endif
