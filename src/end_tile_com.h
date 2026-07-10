#ifndef ENDIAN_BASE_END_TILE_COM_H_
#define ENDIAN_BASE_END_TILE_COM_H_

#include <registry.h>

#define JSMN_HEADER
#include <concord/jsmn.h>

#include "fid.h"

struct end_tile_com {
  int id;
  void* data;
};

struct end_tile_com_ent {
  struct fid fid;
  struct registry tags;
  int (*fillout)(const char* mod_name, const char* namespace_name,
                 const char* file_name, const jsmntok_t* jsmn,
                 const char* json, struct end_tile_com* com);
  // returned pointer will be passed to free()
  char* (*to_json)(const void* elem);
  void (*cleanup)(void* elem);
};

int end_tile_com_fillout(const char* mod_name, const char* namespace_name,
                         const char* file_name, const jsmntok_t* jsmn, const char* json,
                         const char* key, struct end_tile_com* com);
int end_tile_com_cmp(const struct end_tile_com* a, const struct end_tile_com* b);
void end_tile_com_cleanup(struct end_tile_com* elem);

void end_tile_com_ent_load(const char* file_path, const char* mod_name,
                           const char* namespace_name, const char* file_name);
struct end_tile_com_ent* end_tile_com_ent_get(const struct fid* fid);
int end_tile_com_ent_cmp(const struct end_tile_com_ent* a, const struct end_tile_com_ent* b);
void end_tile_com_ent_cleanup(struct end_tile_com_ent* elem);

#endif
