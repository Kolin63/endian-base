#ifndef ENDIAN_BASE_END_VIEW_COMS_H_
#define ENDIAN_BASE_END_VIEW_COMS_H_

#include "end_player.h"

// caller is responsible for freeing returned buf
char* end_view_coms_gen(const struct end_player* player);

#endif
