#mir_tag_start PREPROC
#include <strint.h>
#mir_tag_end

#mir_tag_start end_view_com
#mir_tag_data pos
{
  char* buf = malloc(STRINT_32 + 2 + STRINT_32 + 1);
  char tmp[STRINT_32];

  strcpy(buf, i32_to_str(tmp, player->coms.endian_pos.x, NULL));
  strcat(buf, ", ");
  strcat(buf, i32_to_str(tmp, player->coms.endian_pos.y, NULL));

  return buf;
}
#mir_tag_end
