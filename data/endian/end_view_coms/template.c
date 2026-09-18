ENDIAN_MIRROR_TAG_START(end_view_com);
ENDIAN_MIRROR_TAG_DATA(<< dont forget to set id in tag data >>);
#include <string.h>
{
  char* buf = strdup("hello, world!");
  return buf;
}
ENDIAN_MIRROR_TAG_END();
