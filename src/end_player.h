#ifndef ENDIAN_BASE_PLAYER_H_
#define ENDIAN_BASE_PLAYER_H_

#include <user.h>

#include "end_pos.h"

struct end_player {
  struct user* user;
  struct end_pos pos;
};

int end_player_cmp(struct end_player* const* a, struct end_player* const* b);
void end_player_cleanup(struct end_player** elem);

// initializes player
// returns pointer to player in registry
struct end_player* end_player_init(unsigned long uuid);

// returns pointer to player in registry
// initializes the player if it is not in registry
struct end_player* end_player_get(unsigned long uuid);

void end_player_save(const struct end_player* elem);
void end_player_save_all();

#endif
