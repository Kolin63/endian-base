ENDIAN_MIRROR_TAG_START(PREPROC);
#include <strint.h>
ENDIAN_MIRROR_TAG_END();

ENDIAN_MIRROR_TAG_START(end_view_com);
ENDIAN_MIRROR_TAG_DATA(pos);
{
  char* buf = malloc(STRINT_32 + 2 + STRINT_32 + 1);
  char tmp[STRINT_32];

  strcpy(buf, i32_to_str(tmp, player->coms.endian_pos.x, NULL));
  strcat(buf, ", ");
  strcat(buf, i32_to_str(tmp, player->coms.endian_pos.y, NULL));

  return buf;
}
ENDIAN_MIRROR_TAG_END();
