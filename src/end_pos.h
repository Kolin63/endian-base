#ifndef ENDIAN_BASE_POS_H_
#define ENDIAN_BASE_POS_H_

struct end_sys_pos {
  int x;
  int y;
  int z;
};

struct end_body_pos {
  int x;
  int y;
  int z;
  struct end_sys_pos sys_pos;
};

struct end_pos {
  int x;
  int y;
  struct end_body_pos body_pos;
};

int end_pos_get_z(const struct end_pos* pos);

#endif
