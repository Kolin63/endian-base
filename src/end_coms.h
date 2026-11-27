#ifndef ENDIAN_BASE_COMS_H_
#define ENDIAN_BASE_COMS_H_

// structs defined for each com:
//   struct end_com_%n_%d %t;
// functions for each com:
// void end_com_%n_%d_new(struct end_com_%n_%d* x)
// void end_com_%n_%d_cleanup(struct end_com_%n_%d* x)

// structs defined for each com group:
//   struct end_com_group_%n_%d %n_%d {%t};
// struct has members:
//   struct end_com_%n_%d %n_%d;
// functions for each com group:
// void end_com_group_%n_%d_new(struct end_com_group_%n_%d* coms)
// void end_com_group_%n_%d_cleanup(struct end_com_group_%n_%d* coms)

#include "ref/end_coms.ref.h"
#include "ref/end_com_groups.ref.h"

#endif
