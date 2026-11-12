#ifndef ENDIAN_BASE_END_TAGS_H_
#define ENDIAN_BASE_END_TAGS_H_

// enum has values:
//   END_TAG_UNKNOWN_,
//   END_TAG_%N_%D,
//   END_TAG_AMOUNT_,
enum end_tag;

// struct has members:
//   const bool tags [END_TAG_AMOUNT_];
struct end_tags;

bool end_tag_get_value(const struct end_tags* tags, enum end_tag tag);
enum end_tag end_tag_get(const char* id_str);

#endif
