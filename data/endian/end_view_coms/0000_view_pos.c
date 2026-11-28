#mir_tag_start PREPROC
#include "base/end_pos.h"
#mir_tag_end

// char* end_view_com_%n_%d_gen(const struct end_player* player);
#mir_tag_start end_view_com
#mir_tag_data pos
{
  char* buf = malloc(END_POS_HR_BUF_LEN);
  end_pos_hr(buf, &player->coms.endian_pos);
  return buf;
}
#mir_tag_end
