// https://docs.discord.com/developers/interactions/application-commands

#mir_tag_start PREPROC
#include "base/end_poi.h"
#mir_tag_end

#mir_tag_start command
#mir_tag_data mkpoi
(struct command){
  .type = DISCORD_APPLICATION_CHAT_INPUT,
  .name = "mkpoi",
  .description = "Make a POI",

  .options = (struct command_options){
    .len = 3,
    .arr = (struct command_option[]){
      (struct command_option){
        .type = DISCORD_APPLICATION_OPTION_STRING,
        .name = "type",
        .description = "The POI type",
        .required = true,
      },
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

  .default_member_permissions = 8,
  .owner_only = true,
}
#mir_tag_end

// void (struct discord* client, const struct discord_interaction* event)
#mir_tag_start command_cb
#mir_tag_data mkpoi
{
  int x, y;
  bool x_set, y_set;
  enum end_poi_type_id type = END_POI_UNKNOWN_;

  unsigned long uuid = command_event_get_uuid(event);

  GET_COMMAND_OPTIONS(event) {
    GET_COMMAND_OPTION("type") {
      type = end_poi_type_id_get(value);
      if (type == END_POI_UNKNOWN_) {
        command_basic_response(client, event, true, "Invalid POI type");
        return;
      }
    }
    else GET_COMMAND_OPTION("x") {
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

  if (type == END_POI_UNKNOWN_) {
    command_basic_response(client, event, true, "Invalid POI type (never set)");
    return;
  }

  void* coms = end_poi_coms_init(type);
  struct end_com_endian_pos* pos = end_poi_coms_get_com(type, coms, END_COM_ENDIAN_POS);

  if (pos == NULL) {
    command_basic_response(client, event, true, "POI type has no endian_pos");
    free(coms);
    return;
  }

  pos->x = x;
  pos->y = y;

  struct end_poi* poi = end_poi_add(type, coms);

  if (poi == NULL) {
    command_basic_response(client, event, true, "Error adding POI to registry");
    free(coms);
    return;
  }

  char pos_buf[END_POS_HR_BUF_LEN];
  end_pos_hr(pos_buf, pos);

  char buf[16 + sizeof(pos_buf)];
  strcpy(buf, "Added POI at ");
  strcat(buf, pos_buf);

  struct discord_interaction_response params = {
    .type = DISCORD_INTERACTION_CHANNEL_MESSAGE_WITH_SOURCE,
    .data = &(struct discord_interaction_callback_data){
      .content = buf,
      .flags = DISCORD_MESSAGE_EPHEMERAL,
    },
  };

  discord_create_interaction_response(client, event->id, event->token, &params, NULL);
}
#mir_tag_end
