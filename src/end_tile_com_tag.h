#ifndef ENDIAN_BASE_END_TILE_COM_TAG_H_
#define ENDIAN_BASE_END_TILE_COM_TAG_H_

#include <fid.h>

struct end_tile_com_tag {
  int id;
};

struct end_tile_com_tag_ent {
  struct fid fid;
};

// sets id to -1 if tag doesn't exist
struct end_tile_com_tag end_tile_com_tag_get(const struct fid* fid);
int end_tile_com_tag_cmp(const struct end_tile_com_tag* a, const struct end_tile_com_tag* b);
void end_tile_com_tag_cleanup(struct end_tile_com_tag* elem);

void end_tile_com_tag_ent_load(const char* file_path, const char* mod_name,
                               const char* namespace_name, const char* file_name);
struct end_tile_com_tag_ent* end_tile_com_tag_ent_get(const struct fid* fid);
int end_tile_com_tag_ent_cmp(const struct end_tile_com_tag_ent* a, const struct end_tile_com_tag_ent* b);
void end_tile_com_tag_ent_cleanup(struct end_tile_com_tag_ent* elem);

#endif
