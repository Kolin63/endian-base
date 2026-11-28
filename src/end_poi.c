#include "end_poi.h"

#include <stdlib.h>

#include <log.h>
#include <registry.h>

#include "ref/end_poi_types.ref.h"

// forward decl
void end_poi_cleanup_coms(enum end_poi_type_id type, void* coms);

static struct registry reg;
typedef struct end_poi* end_poi_ptr;

static int
end_poi_cmp(const end_poi_ptr* a, const end_poi_ptr* b) {
  const struct end_poi* a1 = *a;
  const struct end_poi* b1 = *b;

  const struct end_com_endian_pos* a_pos = end_poi_get_com((void*)a1, END_COM_ENDIAN_POS);
  const struct end_com_endian_pos* b_pos = end_poi_get_com((void*)b1, END_COM_ENDIAN_POS);

  if (a_pos == NULL && b_pos == NULL) return 0;
  if (a_pos == NULL) return -1;
  if (b_pos == NULL) return 1;

  const int ax = a_pos->x;
  const int ay = a_pos->y;
  const int bx = b_pos->x;
  const int by = b_pos->y;

  const int y_cmp = (ay < by) * -1 + (ay > by) * 1;
  const int x_cmp = (ax < bx) * -1 + (ax > bx) * 1;

  return (y_cmp != 0) * y_cmp + (y_cmp == 0) * x_cmp;
}

static void
end_poi_cleanup(end_poi_ptr* elem) {
  struct end_poi* poi = *elem;
  end_poi_coms_cleanup(poi->type, poi->coms);
  free(poi->coms);
  free(poi);
}

void
end_poi_reg_init() {
  registry_init(&reg, sizeof(struct end_poi), (void*)end_poi_cmp, (void*)end_poi_cleanup);
}

void
end_poi_reg_cleanup() {
  registry_cleanup(&reg);
}

const struct registry*
end_poi_reg_get() {
  return &reg;
}

struct end_poi*
end_poi_add(enum end_poi_type_id type, void* coms) {
  if (coms == NULL) {
    log_error("Could not add POI; coms are NULL");
    return NULL;
  }
  if (!end_poi_type_verify(type)) {
    log_error("Unknown POI type %i", type);
    return NULL;
  }
  struct end_poi poi = {
    .type = type,
    .coms = coms,
  };
  struct end_com_endian_pos* pos = end_poi_get_com(&poi, END_COM_ENDIAN_POS);
  if (pos == NULL) {
    log_error("Could not add POI; Type %i has no endian_pos", type);
    return NULL;
  }

  struct end_poi* poi_heap = malloc(sizeof(struct end_poi));
  poi_heap->type = type;
  poi_heap->coms = coms;

  // passes all checks:
  registry_add(&reg, &poi_heap);
}

void*
end_poi_coms_get_com(enum end_poi_type_id type, void* coms, enum end_com_id com_id) {
  return end_poi_get_com(&(struct end_poi){.type = type, .coms = coms}, com_id);
}

bool
end_poi_type_verify(enum end_poi_type_id type) {
  return type > END_POI_UNKNOWN_ && type < END_POI_MAX_;
}
