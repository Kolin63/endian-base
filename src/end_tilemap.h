#ifndef ENDIAN_BASE_TILE_MAP_H_
#define ENDIAN_BASE_TILE_MAP_H_

#include <registry.h>

#define JSMN_HEADER
#include <concord/jsmn.h>

struct end_tilemap {
  struct end_tile* tiles;
  unsigned int face_width;
  unsigned int total_tiles;
};

int end_tilemap_fillout(const char* mod_name, const char* namespace_name,
                        const char* file_name, const jsmntok_t* jsmn,
                        const char* json, struct end_tilemap* tilemap);
void end_tilemap_cleanup(struct end_tilemap* elem);

#endif
