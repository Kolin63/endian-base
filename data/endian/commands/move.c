// https://docs.discord.com/developers/interactions/application-commands

#mir_tag_start PREPROC
#include <strint.h>

#include "base/end_player.h"
#include "base/end_pos.h"
#include "endapi/get_command_options.h"
#mir_tag_end

#mir_tag_start command
#mir_tag_data move
(struct command){
  .type = DISCORD_APPLICATION_CHAT_INPUT,
  .name = "move",
  .description = "Move somewhere (see `/lspoi`)",
  .options = (struct command_options){
    .len = 2,
    .arr = (struct command_option[]){
      (struct command_option){
        .type = DISCORD_APPLICATION_OPTION_INTEGER,
        .name = "x",
        .description = "X coordinate (longitude)",
        .required = true,
      },
      (struct command_option){
        .type = DISCORD_APPLICATION_OPTION_INTEGER,
        .name = "y",
        .description = "Y coordinate (latitude)",
        .required = true,
      },
    },
  },
}
#mir_tag_end

// void (struct discord* client, const struct discord_interaction* event)
#mir_tag_start command_cb
#mir_tag_data move
{
  int x, y;
  bool x_set, y_set;

  unsigned long uuid = command_event_get_uuid(event);

  GET_COMMAND_OPTIONS(event) {
    GET_COMMAND_OPTION("x") {
      int err;
      x = str_to_i32(value, &err);
      if (err != STRINT_OK) {
        command_basic_response(client, event, true, "Invalid X coordinate");
        return;
      }
      x_set = true;
    }
    else GET_COMMAND_OPTION("y") {
      int err;
      y = str_to_i32(value, &err);
      if (err != STRINT_OK) {
        command_basic_response(client, event, true, "Invalid Y coordinate");
        return;
      }
      y_set = true;
    }
  }

  if (x_set == false || y_set == false) {
    command_basic_response(client, event, true, "Invalid coordinates");
    return;
  }

  struct end_player* player = end_player_get(uuid);
  struct end_com_endian_pos pos = player->coms.endian_pos;
  pos.x = x;
  pos.y = y;

  char pos_buf[END_POS_HR_BUF_LEN];
  end_pos_hr(pos_buf, &pos);

  char buf[16 + sizeof(pos_buf)];
  strcpy(buf, "Moved to ");
  strcat(buf, pos_buf);

  struct discord_interaction_response params = {
    .type = DISCORD_INTERACTION_CHANNEL_MESSAGE_WITH_SOURCE,
    .data = &(struct discord_interaction_callback_data){
      .content = buf,
    },
  };

  discord_create_interaction_response(client, event->id, event->token, &params, NULL);
}
#mir_tag_end
