// https://docs.discord.com/developers/interactions/application-commands

#mir_tag_start PREPROC
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#include <registry.h>

#include "base/end_poi.h"
#include "base/end_pos.h"
#mir_tag_end

#mir_tag_start command
#mir_tag_data lspois
(struct command){
  .type = DISCORD_APPLICATION_CHAT_INPUT,
  .name = "lspois",
  .description = "List POI's",
  .default_member_permissions = 8,
  .owner_only = true,
}
#mir_tag_end

// void (struct discord* client, const struct discord_interaction* event)
#mir_tag_start command_cb
#mir_tag_data lspois
{
  const size_t max_size = 1024;
  size_t size = 1;
  char buf[max_size + 16];
  strcpy(buf, "POI's:\\n");

  const struct registry* reg = end_poi_reg_get();

  for (int i = 0; i < reg->length; i++) {
    struct end_poi** poi_ptr = registry_itov_safe(reg, i);
    struct end_poi* poi = *poi_ptr;
    if (poi == NULL) continue;

    const char* type = end_poi_type_to_str(poi->type);
    char posbuf[END_POS_HR_BUF_LEN];
    end_pos_hr(posbuf, end_poi_get_com(poi, END_COM_ENDIAN_POS));

    char tmpbuf[strlen(type) + sizeof(posbuf) + 8];
    strcpy(tmpbuf, "- ");
    strcat(tmpbuf, type);
    strcat(tmpbuf, " ");
    strcat(tmpbuf, posbuf);
    strcat(tmpbuf, "\\n");

    size_t tmpsize = strlen(tmpbuf);
    if (size + tmpsize > max_size) {
      strcat(buf, "...\\n");
      break;
    }
    size += tmpsize;
    strcat(buf, tmpbuf);
  }

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
