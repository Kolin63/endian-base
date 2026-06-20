#include "end_api.h"

#include <api.h>

const static struct api* global = NULL;

void end_api_set(const struct api* api) {
  const int got_version = api->get_version();
  if (got_version != API_VERSION) {
    api->log_log(LOG_ERROR, __FILE_NAME__, __LINE__,
                 "Expected API version %i, got version %i",
                 API_VERSION, got_version);
    api->handle_exit();
    return;
  }
  global = api;
}

const struct api* end_api_get() { return global; }

void handle_exit() {
  global->handle_exit();
}

void handle_save() {
  global->handle_save();
}

void abort_cleanup(int code) { global->abort_cleanup(code); }
int get_cleanup_ready() { return global->get_cleanup_ready(); }

CCORDcode discord_create_interaction_response(struct discord* client, u64snowflake interaction_id,
                                              const char interaction_token[],
                                              struct discord_interaction_response* params,
                                              struct discord_ret_interaction_response* ret) {
  return global->discord_create_interaction_response(client, interaction_id, interaction_token, params, ret);
}

void registry_init(struct registry* reg, int val_size, int cmp(const void*, const void*), void cleanup(void* elem)) {
  return global->registry_init(reg, val_size, cmp, cleanup);
}

void registry_cleanup(struct registry* reg) { global->registry_cleanup(reg); }

int registry_safe_cmp(const struct registry* reg, const void* a, const void* b) {
  return global->registry_safe_cmp(reg, a, b);
}

void* registry_add(struct registry* reg, const void* val) { return global->registry_add(reg, val); }

int registry_del_val(struct registry* reg, void* val) { return global->registry_del_val(reg, val); }

int registry_del_key(struct registry* reg, const void* key) { return global->registry_del_key(reg, key); }

int registry_del_i(struct registry* reg, int i) { return global->registry_del_i(reg, i); }

void registry_clear(struct registry* reg) { global->registry_clear(reg); }

void* registry_itov(const struct registry* reg, int i) { return global->registry_itov(reg, i); }

void* registry_itov_safe(const struct registry* reg, int i) { return global->registry_itov_safe(reg, i); }

int registry_ktoi(const struct registry* reg, const void* key) { return global->registry_ktoi(reg, key); }

void* registry_ktov(const struct registry* reg, const void* key) { return global->registry_ktov(reg, key); }

int registry_vtoi(const struct registry* reg, const void* val) { return global->registry_vtoi(reg, val); }

int registry_strcmp(const char* a, const char* b) { return global->registry_strcmp(a, b); }

const struct registry* get_namespace_registry() { return global->get_namespace_registry(); }
const struct namespace* namespace_get(const char* name) { return global->namespace_get(name); }
const struct namespace_mod_entry* namespace_mod_entry_get(const struct namespace* ns, const char* name) {
  return global->namespace_mod_entry_get(ns, name);
}
const struct registry* get_plugin_registry() { return global->get_plugin_registry(); }
const struct plugin* plugin_get(const struct fid* fid) { return global->plugin_get(fid); }
const struct registry* get_function_registry() { return global->get_function_registry(); }
const struct function* function_get(char* name) { return global->function_get(name); }
const struct registry* get_command_registry() { return global->get_command_registry(); }
const struct command* command_get(char* name) { return global->command_get(name); }

void jsmn_iterator_get_string(char* buf, unsigned long size, const char* json, const jsmntok_t* tok) {
  global->jsmn_iterator_get_string(buf, size, json, tok);
}
char* jsmn_iterator_get_string_heap(const char* json, const jsmntok_t* tok) {
  return global->jsmn_iterator_get_string_heap(json, tok);
}
const char* jsmn_iterator_type_to_str(jsmntype_t type) {
  return global->jsmn_iterator_type_to_str(type);
}
void jsmn_iterator_init(struct jsmn_iterator* iter, const jsmntok_t* root, const char* json) {
  global->jsmn_iterator_init(iter, root, json);
}
int jsmn_iterator_next(struct jsmn_iterator* iter) {
  return global->jsmn_iterator_next(iter);
}

char* fileio_read_all(FILE* file) { return global->fileio_read_all(file); }

jsmntok_t* fileio_read_json(const char* json) { return global->fileio_read_json(json); }

struct bot* bot_get_global() { return global->bot_get_global(); }

struct user* user_init(unsigned long uuid) { return global->user_init(uuid); }

struct user* user_get(unsigned long uuid) { return global->user_get(uuid); }

int user_cmp(struct user* const* a, struct user* const* b) { return global->user_cmp(a, b); }

void user_cleanup(struct user** elem) { global->user_cleanup(elem); }

void uuid_to_string(unsigned long uuid, char* buf) { global->uuid_to_string(uuid, buf); }

unsigned long string_to_uuid(const char* str) { return global->string_to_uuid(str); }

int save_write(const char* ns, const char* dir, const char* file, const char* ext, const char* content) {
  return global->save_write(ns, dir, file, ext, content);
}

int save_read(const char* ns, const char* dir, const char* file, const char* ext, char** out) {
  return global->save_read(ns, dir, file, ext, out);
}

struct fid fid_split(char* str) { return global->fid_split(str); }
