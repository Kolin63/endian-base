#include "end_tilemap.h"

#include <concord/jsmn.h>
#include <stdlib.h>
#include <string.h>

#include "end_api.h"
#include "end_tile.h"
#include "fid.h"
#include "jsmn_iterator.h"
#include "json_macros.h"

int end_tilemap_fillout(const char* namespace_name, const char* mod_name,
                        const char* file_name, const jsmntok_t* jsmn,
                        const char* json, struct end_tilemap* tilemap) {
  int error = 0;

  if (tilemap->face_width == 0) {
    log_error("Tilemap face width must be already set (%s:%s:%s)",
              mod_name, namespace_name, file_name);
    error++;
    return error;
  }

  tilemap->total_tiles = (tilemap->face_width * tilemap->face_width) * 6;

  tilemap->tiles = malloc(tilemap->total_tiles * sizeof(struct end_tile));

  struct jsmn_iterator arr_iter;
  jsmn_iterator_init(&arr_iter, jsmn, json);

  while (jsmn_iterator_next(&arr_iter)) {
    END_JSON_CHECK_OBJECT(arr_iter);

    struct jsmn_iterator iter;
    jsmn_iterator_init(&iter, arr_iter.val, json);

    int i = 0;
    while (jsmn_iterator_next(&iter)) {
      struct end_tile* tile = tilemap->tiles + i;
      if (strcmp(iter.key, "id") == 0) {
        char* str = jsmn_iterator_get_string_heap(json, iter.val);
        struct fid fid = fid_split(str);
        if (fid.ns == NULL) {
          log_error("Tile id must be ns:id (%s:%s:%s)", mod_name, namespace_name, file_name);
          error++;
          free(str);
          continue;
        }
        const struct end_tile_ent* tile_ent = end_tile_ent_get(&fid);
        free(str);
      }
    }
  }

  return error;
}

void end_tilemap_cleanup(struct end_tilemap* elem) {
  free(elem->tiles);
}
