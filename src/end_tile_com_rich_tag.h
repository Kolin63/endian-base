#ifndef ENDIAN_BASE_END_TILE_COM_RICH_TAG_H_
#define ENDIAN_BASE_END_TILE_COM_RICH_TAG_H_

#define JSMN_HEADER
#include <concord/jsmn.h>
#include <fid.h>

// a rich tag would be applied to the tile component type, not a specific
// instance of a tile (rather than a component, which is instance specific).
//
// the difference between a rich tag and a tag is that rich tags have data that
// can change between different tile component types

struct end_tile_com_rich_tag {
  int id;
  void* data;
};

struct end_tile_com_rich_tag_ent {
  struct fid fid;
  int (*fillout)(const char* mod_name, const char* namespace_name,
                 const char* file_name, const jsmntok_t* jsmn,
                 const char* json, struct end_tile_com_rich_tag* tag);
  // returned pointer must be passed to free()
  char* (*to_json)(const void* elem);
  void (*cleanup)(void* elem);
};

// sets id to -1 if tag doesn't exist
struct end_tile_com_rich_tag end_tile_com_rich_tag_get(const struct fid* fid);
int end_tile_com_rich_tag_fillout(const char* mod_name, const char* namespace_name,
                                  const char* file_name, const jsmntok_t* jsmn, const char* json,
                                  const char* key, struct end_tile_com_rich_tag* tag);
int end_tile_com_rich_tag_cmp(const struct end_tile_com_rich_tag* a, const struct end_tile_com_rich_tag* b);
void end_tile_com_rich_tag_cleanup(struct end_tile_com_rich_tag* elem);

void end_tile_com_rich_tag_ent_load(const char* file_path, const char* mod_name,
                                    const char* namespace_name, const char* file_name);
struct end_tile_com_rich_tag_ent* end_tile_com_rich_tag_ent_get(const struct fid* fid);
int end_tile_com_rich_tag_ent_cmp(const struct end_tile_com_rich_tag_ent* a, const struct end_tile_com_rich_tag_ent* b);
void end_tile_com_rich_tag_ent_cleanup(struct end_tile_com_rich_tag_ent* elem);

#endif
