#ifndef ENDIAN_BASE_RICH_TAGS_H_
#define ENDIAN_BASE_RICH_TAGS_H_

// structs defined for each rich_tag:
//   struct end_rich_tag_%n_%d %t;

// struct has members:
//   struct end_rich_tag_%n_%d %n_%d;
struct end_rich_tags;

void end_rich_tags_cleanup(struct end_rich_tags* rich_tags);

#include "ref/end_rich_tags.ref.h"

#endif
