#ifndef ENDIAN_BASE_END_TILE_H_
#define ENDIAN_BASE_END_TILE_H_

#include "fid.h"
#include "registry.h"

struct end_tile {
  int id;
  struct registry com;
};

struct end_tile_ent {
  struct fid fid;
  char* name;
  char* desc;
  char* icon;
  struct registry coms;
};

void end_tile_ent_load(const char* file_path, const char* namespace_name,
                       const char* mod_name, const char* file_name);
struct end_tile_ent* end_tile_ent_get(const struct fid* fid);
int end_tile_ent_cmp(const struct end_tile_ent* a, const struct end_tile_ent* b);
void end_tile_ent_cleanup(struct end_tile_ent* elem);

const struct end_tile_ent* end_tile_get_ent(const struct end_tile* tile);

#endif
