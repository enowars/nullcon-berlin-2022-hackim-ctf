/* Config */
#define STORAGE_DIR "../data/"
#define COOKIE_DIR STORAGE_DIR "cookies/"
#define DATA_DIR STORAGE_DIR "data/"
#define USER_DIR STORAGE_DIR "users/"
#define MSG_DIR STORAGE_DIR "msgs/"
#define PRIORITY_MSG_DIR STORAGE_DIR "priority_msgs/"

#define INI_LEN_MAX (256)
#define DELIM ('=')
#define KV_SPLIT ('&')
#define KV_START ('?')
#define COOKIE_NAME "web1.0code"
#define NL "\r\n"
#define QUERY_COUNT (32)
#define NONCE_LEN (20)
#define ADMIN_CHALLENGE_LEN (16)
#define MSG_TOKEN_LEN (8)
#define COOKIE_LEN (10)
#define PRUNE_TIME "15"
#define CONST_MATCH_SECS (5)
#define ADMIN "admin"

/* CoMmON DaTAtYpeS */
#define STR(x) ((char *)x)
#define NIL ((void *)0)
#define KV char **

/* The web 1.0 framwork */
#define BODY() readline(0)
#define FILE_NEXT() readline(file)

#define STATE ((state_t *)state)
#define NONCE (STATE->nonce)
#define IS_GET (!strcmp(STATE->method, "GET"))
#define IS_POST (!strcmp(STATE->method, "POST"))

#define PFATAL(x...)                                    \
  do {                                                  \
                                                        \
    fprintf(stderr, "%d ERR :%04d: ", runid, __LINE__); \
    fprintf(stderr, x);                                 \
    perror("\1");                                       \
    fflush(stdout);                                     \
    fflush(stderr);                                     \
    abort();                                            \
                                                        \
  } while (0);

#define PLOG(x...)                                      \
  do {                                                  \
                                                        \
    fprintf(stderr, "%d PLG :%04d: ", runid, __LINE__); \
    fprintf(stderr, x);                                 \
    perror("\1");                                       \
    fflush(stderr);                                     \
                                                        \
  } while (0);

#define LOG(x...)                                       \
  do {                                                  \
                                                        \
    fprintf(stderr, "%d LOG :%04d: ", runid, __LINE__); \
    fprintf(stderr, x);                                 \
    fflush(stderr);                                     \
                                                        \
  } while (0);

#define KV_FOREACH(kv, block)              \
  do {                                     \
                                           \
    auto idx = 0;                          \
    auto key_idx = 0;                      \
    auto val_idx = 1;                      \
    auto key, val;                         \
    KV   cur = (KV)(kv);                   \
    while (cur[key_idx] && cur[val_idx]) { \
                                           \
      key = cur[key_idx];                  \
      val = cur[val_idx];                  \
      {block};                             \
      idx++;                               \
      key_idx += 2;                        \
      val_idx += 2;                        \
                                           \
    }                                      \
                                           \
  } while (0);

#define FILE_KV_FOREACH(filename, block)             \
  do {                                               \
                                                     \
    auto file = fopen(filename, "r");                \
    if (!file) {                                     \
                                                     \
      LOG("Error accessing KV file %s\n", filename); \
                                                     \
    } else {                                         \
                                                     \
      do {                                           \
                                                     \
        KV query = parse_query(FILE_NEXT());         \
        if (!query) { break; }                       \
        KV_FOREACH(query, {block});                  \
                                                     \
      } while (1);                                   \
                                                     \
                                                     \
      fclose(file);                                  \
                                                     \
    }                                                \
                                                     \
  } while (0);

#define ROUTE(method_name, route_name)        \
  if (!strcmp(STATE->method, #method_name) && \
      !strcmp(STATE->route, #route_name)) {   \
                                              \
    handle_##route_name(state);               \
    handled = 1;                              \
                                              \
  }

/* 0-9A-Za-z */
#define IS_ALPHANUMERIC(c)                                   \
  (((c) >= '0' && (c) <= '9') || ((c) >= 'A' && c <= 'Z') || \
   ((c) >= 'a' && (c) <= 'z'))

/* Templating in C is eas-C */
#define TEMPLATE(x) #x

/* Variables */
extern stdin;
extern stdout;
extern stderr;
static char empty_list[2][2] = {0};
static sel[2] = {1};
static runid;

typedef struct state {

  char *cookie;
  char *nonce;

  char *username;

  char *user_loc;
  char *cookie_loc;

  char *method;
  char *route;

  int logged_in;

  KV queries[QUERY_COUNT];

} state_t;

