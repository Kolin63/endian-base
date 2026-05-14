#ifndef ENDIAN_BASE_END_API_H_
#define ENDIAN_BASE_END_API_H_

#include <api.h>

void end_api_set(const struct api* api);
const struct api* end_api_get();

#define log_trace(...) end_api_get()->log_log(LOG_TRACE, ENDIAN_DEF_STR(MOD_NAME) ":" __FILE_NAME__, __LINE__, __VA_ARGS__)
#define log_debug(...) end_api_get()->log_log(LOG_DEBUG, ENDIAN_DEF_STR(MOD_NAME) ":" __FILE_NAME__, __LINE__, __VA_ARGS__)
#define log_info(...) end_api_get()->log_log(LOG_INFO, ENDIAN_DEF_STR(MOD_NAME) ":" __FILE_NAME__, __LINE__, __VA_ARGS__)
#define log_warn(...) end_api_get()->log_log(LOG_WARN, ENDIAN_DEF_STR(MOD_NAME) ":" __FILE_NAME__, __LINE__, __VA_ARGS__)
#define log_error(...) end_api_get()->log_log(LOG_ERROR, ENDIAN_DEF_STR(MOD_NAME) ":" __FILE_NAME__, __LINE__, __VA_ARGS__)
#define log_fatal(...) end_api_get()->log_log(LOG_FATAL, ENDIAN_DEF_STR(MOD_NAME) ":" __FILE_NAME__, __LINE__, __VA_ARGS__)

// attempts exit. if cleanup is not ready, function returns
void handle_exit();
// premature cleanup before everything is initialized, for example to abort
// the program during the cli args initialization phase
void abort_cleanup(int code);
// returns 0 if cleanup is not ready, 1 if it is. set to 1 after mod loading
// and init/load functions are called
int get_cleanup_ready();

CCORDcode discord_create_interaction_response(struct discord* client, u64snowflake interaction_id,
                                              const char interaction_token[],
                                              struct discord_interaction_response* params,
                                              struct discord_ret_interaction_response* ret);

// puts a new registry on the heap. registry_cleanup() must be called when it
// is done being used
struct registry* registry_init(int val_size,
                               int cmp(const void*, const void*),
                               void cleanup(void* elem));

// frees allocated memory for a registry. if the registry contains structs
// with data on the heap, those fields must be freed before calling this
// function
void registry_cleanup(struct registry* reg);

// calls the registry's cmp function. elides function call if either a or b
// are NULL. will segfault if the cmp function is unset.
int registry_safe_cmp(const struct registry* reg, const void* a, const void* b);

// adds a value. returns pointer to value in registry, or NULL if key already
// exists
void* registry_add(struct registry* reg, const void* val);

// removes all entries from registry. does not call registry_cleanup(). does
// not need to be called before calling registry_cleanup()
void registry_clear(struct registry* reg);

// index to value. no bounds checking
void* registry_itov(const struct registry* reg, int i);

// index to value. returns NULL on error
void* registry_itov_safe(const struct registry* reg, int i);

// key to index. returns -1 if the key doesn't exist
int registry_ktoi(const struct registry* reg, const void* key);

// key to value. returns -1 if the key doesn't exist
void* registry_ktov(const struct registry* reg, const void* key);

// fast implementation of strcmp. only return values are 1, 0, or -1
int registry_strcmp(const char* a, const char* b);

const struct registry* get_namespace_registry();
const struct namespace* namespace_get(const char* name);
const struct namespace_mod_entry* namespace_mod_entry_get(const struct namespace* ns, const char* name);
const struct registry* get_plugin_registry();
const struct plugin* plugin_get(char* namespace, char* name);
const struct registry* get_function_registry();
const struct function* function_get(char* name);
const struct registry* get_command_registry();
const struct command* command_get(char* name);

struct bot* bot_get_global();

// initializes user
// returns pointer to user in registry
struct user* user_init(unsigned long uuid);

// returns pointer to user in registry
// initializes the user if it is not in registry
struct user* user_get(unsigned long uuid);

int user_cmp(struct user* const* a, struct user* const* b);

void user_cleanup(struct user** elem);

// converts uuid (unsigned long) to string.
// string should be of length UUID_STR_LEN
void uuid_to_string(unsigned long uuid, char* buf);

// converts string to uuid (unsigned long)
unsigned long string_to_uuid(const char* str);

// writes to save file. returns 0 if ok.
// dir does not need a trailing slash
// ext is file extension, and it should not include the dot. for example, a
// json file has the extension "json", not ".json"
int save_write(const char* dir, const char* file, const char* ext, const char* content);

// reads from save file into out. returns 0 if ok
// dir does not need a trailing slash
// ext is file extension, and it should not include the dot. for example, a
// json file has the extension "json", not ".json"
int save_read(const char* dir, const char* file, const char* ext, char** out);

#endif
