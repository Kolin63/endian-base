#include "end_pos.h"

#include <strint.h>

#include <string.h>

void
end_pos_hr(char buf[END_POS_HR_BUF_LEN], const struct end_pos* pos) {
  char x[STRINT_32];
  char y[STRINT_32];

  i32_to_str(x, pos->x, NULL);
  i32_to_str(y, pos->y, NULL);

  strcpy(buf, "(");
  strcat(buf, x);
  strcat(buf, ", ");
  strcat(buf, y);
  strcat(buf, ")");
}
