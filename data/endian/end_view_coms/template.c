#mir_tag_start PREPROC
#include <string.h>
#mir_tag_end

// char* end_view_com_%n_%d_gen(const struct end_player* player);
#mir_tag_start end_view_com
#mir_tag_data <<SET_ID>>
{
  char* buf = strdup("hello, world!");
  return buf;
}
#mir_tag_end
