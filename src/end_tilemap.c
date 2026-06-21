#include "end_tilemap.h"

#include <concord/jsmn.h>
#include <stdlib.h>

#include "end_api.h"
#include "end_tile.h"
#include "jsmn_iterator.h"
#include "json_macros.h"

int end_tilemap_fillout(const char* mod_name, const char* namespace_name,
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

  struct jsmn_iterator iter;
  jsmn_iterator_init(&iter, jsmn, json);

  for (int i = 0; jsmn_iterator_next(&iter); i++) {
    END_JSON_CHECK_OBJECT(iter);
    struct end_tile* tile = tilemap->tiles + i;
    error += end_tile_fillout(mod_name, namespace_name, file_name, iter.val, json, tile);
  }

  return error;
}

void end_tilemap_cleanup(struct end_tilemap* elem) {
  if (elem->tiles != NULL) {
    for (unsigned int i = 0; i < elem->total_tiles; i++) {
      end_tile_cleanup(elem->tiles + i);
    }
    free(elem->tiles);
  }
}
