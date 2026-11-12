#ifndef ENDIAN_BASE_POS_H_
#define ENDIAN_BASE_POS_H_

#include "base/end_coms.h"

struct end_com_endian_pos;
#define end_pos end_com_endian_pos

#define END_POS_HR_BUF_LEN 32

// formats pos into string of (x, y). buf must be length END_POS_HR_BUF_LEN.
void end_pos_hr(char buf[END_POS_HR_BUF_LEN], const struct end_pos* pos);

#endif
