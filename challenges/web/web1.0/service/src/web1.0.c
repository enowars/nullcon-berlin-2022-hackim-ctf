/*
  There is this new language called python.
  We have python at home:
*/

#include "web1.0.h"

/* Sane code STARTS with main. Why would anybody read from bottom to top? */
main() {

  runid = *((int *)rand_str(4));

#ifndef TEST

  /* clang-format off */
  #ifndef NO_ALARM
  alarm(15);
  #endif
  
  https://www.openroad.org/cgi-bin/cgienvdemo

  LOG("Welcome to web1.0.\n");

  auto current_cookies = getenv("HTTP_COOKIE");
  auto request_method = getenv("REQUEST_METHOD");
  auto query_string = getenv("QUERY_STRING");
  /* Webserver name to this binary */
  auto script_name = getenv("SCRIPT_NAME");

  auto cookie_kv = parse_query(current_cookies);

  auto cookie = "";
  /* clang-format on */
  KV_FOREACH(cookie_kv, {

    if (!strcmp(key, COOKIE_NAME)) {

      auto i;
      cookie = val;
      for (i = 0; STR(cookie)[i]; i++) {

        if (STR(cookie)[i] == ';' || STR(cookie)[i] == ' ') {

          STR(cookie)[i] = '\0';
          break;

        }

      }

      LOG("Got cookie %s\n", cookie);

    }

  });

  auto state = init_state(request_method, cookie, query_string);

  write_headers(state);

  /* AJAX State of mind */
  if IS_GET { write_head(state); }

  LOG("REQUEST_METHOD='%s' HTTP_COOKIE='web1.0code=%s' QUERY_STRING='%s' .%s\n",
      STATE->method, STATE->cookie, query_string, script_name);

  auto handled = 0;

  ROUTE(GET, index);
  ROUTE(POST, login);
  ROUTE(POST, register);

  ROUTE(GET, dispense);
  ROUTE(GET, reserve);
  ROUTE(POST, reserve);

  ROUTE(GET, msg);
  ROUTE(POST, msg);
  ROUTE(PUT, msg);

  if (!handled) {

    LOG("Unknown route!\n");
    auto error = "Unsupported route or method!";
    printf(
  #include <error.templ>
    );

  }

  if IS_GET { printf(NL "</html>" NL); }

  fflush(stdin);
  LOG("Finished %s %s - %s %s \n", STATE->method, STATE->route, query_string,
      script_name);

  return 0;

#else  // TESTs
  #include "unittests.h"
  tests();
#endif

}

assert(condition) {

  if (!condition) {

    LOG("Assert failed :/\n");
    fflush(stdout);
    trigger_gc(1);
    exit(1);

  }

}

/* Frees all memory we no longer need */
trigger_gc(code) {

  exit(code);

}

/* The os will free our memory. */
__asan_default_options() {

  /* The os will free our memory. */
  return "detect_leaks=0";

}

escape(replace, str) {

  auto i;
  auto len = strlen(str);
  auto written = 0;
  /* all the right values for all the wrong reasons */
  auto replace_len = ((strlen(replace) + 4));
  auto ret = calloc(1, len * replace_len);
  /* LOG("len %d, replen %d, rep %s, str %s, ptr %p\n", len, replace_len,
   * replace, str, ret); */
  for (i = 0; i < len; i++) {

    /* LOG("%d %c %s", written, str[i], ret); */
    written += sprintf(STR(ret) + written, replace, STR(str)[i]);

  }

  /* LOG("\n"); */

  return ret;

}

#define E4(name, replace) \
  escape_4_##name(str) { return escape(replace, str); }

E4(py, "\\x%2x")
E4(html, "&#x%2x;")
E4(hash, "%2x")

file_create_atomic(filename) {

/* Constants for open */
#define O_WRONLY 01
#define O_CREAT 0100
#define O_EXCL 0200
#define S_IWUSR 0200
#define S_IRUSR 0400

  auto fd = open(filename, O_CREAT | O_WRONLY | O_EXCL, S_IRUSR | S_IWUSR);

  if (fd < 0) {

    PLOG(filename);
    return NIL;

  }

  LOG("Created file: %s\n", filename);

  return fdopen(fd, "w");

}

/* A random char in 0-9A-Za-z */
get_rand_alphanumberic() {

  auto ret = 0;
  if (!getrandom(&ret, 1, 0)) { PFATAL("Couldn't get random"); }
  if (IS_ALPHANUMERIC(ret)) { return ret; }
  return get_rand_alphanumberic();

}

/* A new string with only alphanumeric chars.
   The others are stripped. */
dup_alphanumeric(str) {

  auto i;
  auto retpos = 0;
  if (!str) { return NIL; }
  auto ret = calloc(1, 1024);
  for (i = 0; STR(str)[i]; i++) {

    if (IS_ALPHANUMERIC(STR(str)[i])) { STR(ret)[retpos++] = STR(str)[i]; }

  }

  return ret;

}

/* returns a random string with the given length */
rand_str(len) {

  auto i;
  auto ret = calloc(2, len);
  if (!ret) { PFATAL("calloc") };
  for (i = 0; i < len; i++) {

    STR(ret)[i] = get_rand_alphanumberic();

  }

  return ret;

}

parse_query(str) {

  auto i;

  if (!str) { return NIL; }
  auto content_len = strlen(str);
  if (!content_len) { return empty_list; }

  KV   ret = calloc(1, (content_len)*4 + 8);
  auto contents = strdup(str);
  auto parsing_key = 1;
  auto current_len = 0;
  ret[0] = contents;
  auto val_count = 0;

  /* Strip tailing newline */
  if (STR(contents)[content_len - 1] == '\n') {

    content_len = content_len - 1;
    STR(contents)[content_len] = '\0';

  }

  /* parse */
  for (i = 0; i < content_len; i++) {

    if (!STR(contents)[i]) {

      ret[++val_count] = "";
      return ret;

    } else if ((STR(contents)[i] == (parsing_key ? DELIM : KV_SPLIT)) &&

               current_len) {

      STR(contents)[i] = 0;
      ret[++val_count] = &(STR(contents)[i + 1]);
      parsing_key = !parsing_key;
      current_len = 0;

    } else {

      current_len++;

    }

  }

  return ret;

}

read_ini(filename) {

  auto i;
  auto ini_pos = 0;
  auto key_exists;
  auto keys[128] = {0};
  auto linec = 0;
  KV   ini = calloc(1, 256);

  FILE_KV_FOREACH(filename, {

    key_exists = 0;
    for (i = 0; keys[i]; i++) {

      if (!strcmp(key, keys[i])) { key_exists = 1; }

    }

    if (!key_exists) {

      ini[ini_pos++] = key;
      ini[ini_pos++] = val;

    }

  });

  return ini;

}

write_ini(filename, ini) {

  auto i;
  auto key_exists;
  auto keys[128] = {0};
  auto linec = 0;
  auto file = fopen(filename, "w");
  if (!file) { PFATAL("Couldn't open ini file"); }
  KV_FOREACH(ini, {

    key_exists = 0;
    for (i = 0; keys[i]; i++) {

      if (!strcmp(key, keys[i])) { key_exists = 1; }

    }

    if (!key_exists) {

      LOG("Outputting %s=%s\n", key, val);

      if (fprintf(file, "%s=%s\n", key, val) < 0) {

        PLOG("Writing ini");
        trigger_gc(1);

      }

      keys[i] = key;

    }

  });

  return fclose(file);

}

debug_print_query(query) {

  fprintf(stderr, "---> Query:\n");
  KV_FOREACH(query, { fprintf(stderr, "%s=%s\n", key, val); });
  fprintf(stderr, "<--- EOQ\n");
  return fflush(stderr);

}

#define LOC(name, DIR)                  \
  loc_##name(locname) {                 \
                                        \
    auto loc = calloc(1, 1032);         \
    sprintf(loc, "%s%s", DIR, locname); \
    return loc;                         \
                                        \
  }

LOC(cookie, COOKIE_DIR)
LOC(user, USER_DIR)

file_set_val(filename, key_to_write, val_to_write) {

  LOG("Setting %s to %s\n", key_to_write, val_to_write);
  auto keycpy = strdup(key_to_write);
  auto valcpy = strdup(val_to_write);

  KV ini = read_ini(filename);

  auto wrote_val = 0;
  auto last_idx = -1;

  KV_FOREACH(ini, {

    if (!strcmp(key, keycpy)) {

      ini[val_idx] = valcpy;
      wrote_val = 1;

    }

    last_idx = val_idx;

  });

  if (!wrote_val) {

    ini[last_idx + 1] = keycpy;
    ini[last_idx + 2] = valcpy;

  } else {

    free(keycpy);

  }

  write_ini(filename, ini);
  return 0;

}

get_val(state, key_to_find) {

  auto i;

  for (i = 0;; i++) {

    /*LOG("p: %d %p\n"NL, i, state->queries[i]);*/
    if (!STATE->queries[i]) { STATE->queries[i] = parse_query(BODY()); }

    if (!STATE->queries[i]) { return NIL; }
    /*LOG("query: %s\n"NL, state->queries[i]);*/
    KV_FOREACH(STATE->queries[i], {

      /*LOG("tofind: %s - %s %s\n", key_to_find, key, val);*/
      if (!strcmp(key, key_to_find)) {

        auto len = strlen(val);
        if (STR(val)[len - 1] <= ' ') { STR(val)[len - 1] = '\0'; }
        return val;

      }

    });

  }

  dprintf(2, "Getval without return should never be reached\n");
  assert(0);

}

file_get_val(filename, key_to_find, default_val) {

  auto ini = read_ini(filename);
  KV_FOREACH(ini, {

    if (!strcmp(key, key_to_find)) { return val; }

  });

  return default_val;

}

write_headers(state) {

  write_palmtree();
  printf(
      "Content-Security-Policy: script-src 'nonce-%s'; style-src 'nonce-%s'"
      " https://fonts.googleapis.com/css2?family=Lobster&display=swap;" NL
      "X-Frame-Options: SAMEORIGIN" NL "X-Xss-Protection: 1; mode=block" NL
      "X-Content-Type-Options: nosniff" NL
      "Referrer-Policy: no-referrer-when-downgrade" NL
      "Feature-Policy "
      "geolocation 'self'; midi 'self'; sync-xhr 'self'; microphone 'self'; "
      "camera 'self'; magnetometer 'self'; gyroscope 'self'; speaker 'self'; "
      "fullscreen *; payment 'self';" NL "Content-Type: text/html" NL

      "Cache-Control: no-store" NL "Set-Cookie: " COOKIE_NAME
      "=%s; HttpOnly" NL NL,
      NONCE, NONCE, STATE->cookie);
  return 0;

}

write_palmtree() {

  printf("Palm-0: |       __ _.--..--._ _" NL
         "Palm-1: |    .-' _/   _/\\_   \\_'-." NL
         "Palm-2: |   |__ / 🦜 _/\\__/\\_   \\__|" NL
         "Palm-3: |      |___/\\_\\__/  \\___|" NL
         "Palm-4: |             \\__/     " NL
         "Palm-5: |              \\__/ " NL
         "Palm-6: |               \\__/        |>" NL
         "Palm-7: |       __🥥____.~\\__/~.____|" NL
         "Palm-8: |     /  ENO                 \\" NL
         "Palm-9: |~~~~~~  ~~~~~ ~~!~~  ~~~ 🌊🌊~ ~~~" NL);

}

write_head(state) {

  auto palm = head_palm();
  printf(
#include <head.templ>
  );

  return 0;

}

head_palm() {

  return "<meta name=palm content=\"\n "
         "......................................__\n "
         ".............................,-~*`¯lllllll`*~,\n "
         ".......................,-~*`lllllllllllllllllllllllllll¯`*-,\n "
         "..................,-~*llllllllllllllllllllllllllllllllllllllllllll*"
         "-,"
         "\n "
         "...............,-*"
         "llllllllllllllllllllllllllllllllllllllllllllllllll."
         "\\\n "
         ".............;*`lllllllllllllllllllllllllll,-~*~-,"
         "llllllllllllllllllll\\\n "
         "..............\\llllllllllllllllllllllll/"
         ".........\\ENOlllllllllll,-`~-,\n "
         "...............\\lllllllllllllllllllll,-*...........`~-~-,...(.(¯`*"
         ",`"
         ",\n "
         "................\\llllllllllll,-~*.....................)_-\\..*`*;."
         ".)"
         "\n "
         ".................\\,-*`¯,*`)............,-~*`~................/\n "
         "..................|/.../.../~,......-~*,-~*`;................/"
         ".\\\n "
         "................./.../.../.../"
         "..,-,..*~,.`*~*................*...\\\n "
         "................|.../.../.../"
         ".*`...\\...........................)....)¯`~,\n "
         "................|./.../..../.......)......,.)`*~-,............/"
         "....|..)...`~-,\n "
         "..............././.../...,*`-,.....`-,...*`....,---......\\..../"
         "...../..|.........¯```*~-,,,,\n "
         "...............(..........)`*~-,....`*`.,-~*.,-*......|.../..../"
         ".../"
         "............\\........\n "
         "................*-,.......`*-,...`~,..``.,,,-*..........|.,*...,*.."
         ".|"
         "..............\\........\n "
         "...................*,.........`-,...)-,..............,-*`...,-*...."
         "(`"
         "-,............\\.......\n "
         "......................f`-,.........`-,/"
         "...*-,___,,-~*....,-*......|...`-,..........\\........\n \">";

}

init_state(request_method, current_cookie, query_string) {

  auto i;
  auto state = calloc(sizeof(state_t), 1);

  if (!request_method || !(STR(request_method)[0])) {

    LOG("No request method provided. Assuming GET\n");
    STATE->method = "GET";

  } else {

    STATE->method = request_method;

  }

  auto new_cookie = dup_alphanumeric(current_cookie);
  if (new_cookie && !STR(new_cookie)[0]) {

    free(new_cookie);
    new_cookie = NIL;

  }

  if (!new_cookie) {

    /* A new browser, welcome! :) */
    new_cookie = rand_str(COOKIE_LEN);

  }

  STATE->cookie = new_cookie;
  STATE->cookie_loc = loc_cookie(STATE->cookie);

  auto file = file_create_atomic(STATE->cookie_loc);
  if (file) {

    LOG("Existing cookie %s\n", STATE->cookie);
    fclose(file);

  }

  STATE->logged_in = !strcmp(cookie_get_val(state, "logged_in", "0"), "1");
  LOG("User is%s logged in.\n", STATE->logged_in ? "" : " not");

  if (STATE->logged_in) {

    STATE->username = cookie_get_val(state, "username", NIL);

  }

  if (STATE->username) {

    LOG("User %s is back!\n", STATE->username);
    STATE->user_loc = loc_user(STATE->username);

  } else {

    STATE->username = "New User";

  }

  STATE->nonce = rand_str(NONCE_LEN);

  maybe_prune(STATE, COOKIE_DIR);
  maybe_prune(STATE, USER_DIR);

  STATE->route = "index";

  if (query_string) {

    STATE->queries[0] = parse_query(query_string);
    KV_FOREACH(STATE->queries[0], {

      if (!strcmp(key, "route")) { STATE->route = val; }

    })

    if (STATE->route[0]) { LOG("Route: %s\n", STATE->route); }

  } else {

    query_string = "";

  }

  return state;

}

parse_cookie(cookies) {

  auto i;

  if (!cookies) { return NIL; }
  auto content_len = strlen(cookies);
  if (!content_len) { return NIL; }

  auto contents = strdup(cookies);
  auto parsing_key = 1;
  auto current_len = 0;
  auto current_key = contents;
  auto current_val = NIL;
  auto val_count = 0;

  /* Strip tailing newline */
  if (STR(contents)[content_len - 1] == '\n') {

    content_len = content_len - 1;
    STR(contents)[content_len] = '\0';

  }

  for (i = 0; i < content_len; i++) {

    if (STR(contents)[i] == ';') {

      STR(contents)[i] = '\0';
      if (parsing_key) {

        current_key = STR(contents) + i + 1;
        current_val = 0;

      } else {

        if (!strcmp(current_key, COOKIE_NAME)) {

          return dup_alphanumeric(current_val);

        }

        parsing_key = 1;
        current_key = STR(contents) + i + 1;
        current_val = NIL;

      }

    } else if (parsing_key && STR(contents)[i] == ' ') {

      current_key = STR(contents) + i + 1;

    } else if (parsing_key &&

               (STR(contents)[i] == DELIM || STR(contents)[i] == ' ')) {

      STR(contents)[i] = '\0';
      parsing_key = 0;
      current_val = STR(contents) + i + 1;

    }

  }

  if (!strcmp(current_key, COOKIE_NAME)) {

    return dup_alphanumeric(current_val);

  }

  return NIL;

}

csrf_new(state) {

  auto csrf = rand_str(8);
  cookie_set_val(state, "csrf", csrf);
  return csrf;

}

csrf_validate(state) {

#ifdef NO_CSRF
  return 1;
#endif

  auto csrf_sent = get_val(state, "csrf");
  auto csrf_stored = cookie_get_val(state, "csrf", NIL);
  if (!csrf_stored) {

    LOG("INTERNAL_ERROR: No valid csrf token could be found for cookie %s!\n",
        STATE->cookie);
    abort();
    return 0;

  }

  if (!STR(csrf_stored)[0]) {

    LOG("Tried to write data without requestion CSRF token first\n");
    return 0;

  }

  cookie_set_val(state, "csrf", "");
  if (strcmp(csrf_sent, csrf_stored)) {

    LOG("CSRF Validation failed, expected %s (len %d) but got %s (len %d)\n",
        csrf_stored, strlen(csrf_stored), csrf_sent, strlen(csrf_sent));
    return 0;

  }

  return 1;

}

handle_index(state) {

  /*auto cf = cookie_file(cookie);*/
  /*read_ini(USER_DIR + username);*/

  auto username = STATE->username;
  auto logged_in = STATE->logged_in;

  auto csrf = csrf_new(state);

  printf(
#include "body_index.templ"
  );

  return 0;

}

split(str, splitter) {

  auto i;

  if (!str) { return empty_list; }
  auto len = strlen(str);
  if (!len) { return empty_list; }
  KV   ret = calloc(sizeof(char *), len / 2);
  auto pos = 0;
  ret[pos] = str;
#define CURRENT_ITEM ret[pos]
  for (i = 0; STR(str)[i]; i++) {

    if (STR(str)[i] == splitter) {

      STR(str)[i] = '\0';
      if (strlen(ret[pos])) {

        /* LOG("Found element:: %s\n", CURRENT_ITEM); */
        pos++;

      }

      CURRENT_ITEM = STR(str) + i + 1;

    }

  }

#undef CURRENT_ITEM
  if (!strlen(ret[pos])) {

    ret[pos] = NIL;
    pos--;

  }

  LOG("Split item count for %s: %d\n", str, pos);
  return ret;

}

/* reads a line */
readline(f) {

  char buf[100001];
  if (!(!f ? gets(buf) : fgets(buf, sizeof(buf), f))) {

    /* Looks like EOF to me */
    return NIL;

  }

  auto ret = malloc(strlen(buf) + 2);
  strcpy(ret, buf);
  return ret;

}

own_msg_list(state) {

  auto i;

  auto own_msgs = get_user_val(state, "msgs", "");
  LOG("User msgs: %s\n", own_msgs);
  return split(own_msgs, '/');

}

ls(state, dir) {

  auto i;

  /* prune all (26 + 26 + 10) requests */
  maybe_prune(state, dir);
  /* using forward slash as divider = never a valid unix filename */
  auto list_str = run("ls -t '%s' | tr '\\n' '/' | head -c 99999", dir);
  return split(list_str, '/');

}

maybe_prune(state, dir) {

  static prune_offset = 0;

  if (STATE->nonce[0] == 'A' + (prune_offset++ % 20)) {

    LOG("Pruning %s this time (every (26+26+10)th time).\n", dir);
    prune(dir);

  }

}

prune(dir) {

  LOG("Pruning all files in %s older than " PRUNE_TIME " minutes\n", dir);
  /* mmin -> motification time, amin -> access time */
  // ! -name admin -> exclude files called admin
  LOG(run("find '%s' -mmin +" PRUNE_TIME " -type f ! -name " ADMIN " -delete",
          dir));

}

render_own_msgs(state) {

  KV msg_list = own_msg_list(state);
  if (!msg_list || !msg_list[0]) { return ""; }
  return render_msg_template(state, msg_list, 0);

}

render_all_msgs(state) {

  KV msg_list = ls(state, MSG_DIR);
  if (!msg_list || !msg_list[0]) { return ""; }
  return render_msg_template(state, msg_list, 1);

}

render_msg_template(state, msg_list, highlight_priority_msgs) {

  auto i;
  KV   priority_msgs = NIL;
  if (highlight_priority_msgs) { priority_msgs = ls(state, PRIORITY_MSG_DIR); }

  auto ret = calloc(1, 1024000);
  auto retpos = 0;

#define CURRENT_MSG (((KV)msg_list)[i])
  for (i = 0; CURRENT_MSG; i++) {

    auto k;
    auto priority_msg = 0;
    auto priority_msg_admin = "";
    auto msg_name = CURRENT_MSG;
    auto msg_len = strlen(CURRENT_MSG);
    for (k = 0; priority_msgs && priority_msgs[k]; k++) {

      /*  LOG("Priority msg %s\n", priority_msgs[k]); */

      if (!strncmp(msg_name, priority_msgs[k], msg_len)) {

        priority_msg = 1;
        /* +1 because we used an underscore as divider ([token]_[name]) */
        priority_msg_admin = priority_msgs[k] + msg_len + 1;
        if (!priority_msg_admin) { priority_msg_admin = ""; }
        /* LOG("Msg %s is a priority msg, owner: %s\n", msg_name,
            priority_msg_admin); */
        break;

      }

    }

    /* LOG("Current msgname: %s, ret: %s @%p-%p\n", msg_name, ret, ret, ret
     * + retpos); */

    retpos += sprintf(STR(ret) + retpos,
#include <msg.templ>
    );

    if (retpos > 1000000) break;

  }

#undef CURRENT_MSG

  return ret;

}

cookie_get_val(state, key, default_val) {

  return file_get_val(STATE->cookie_loc, key, default_val);

}

cookie_set_val(state, key, val) {

  return file_set_val(STATE->cookie_loc, key, val);

}

file_delete(filename) {

  return remove(filename);

}

/* returns 0 on error / if :user exists */
user_create(name, pass) {

  // clang-format off
  auto user_loc = loc_user(name);
  auto file = file_create_atomic(user_loc);

  https://stackoverflow.com/questions/230062/whats-the-best-way-to-check-if-a-file-exists-in-c

  if (!file) {

    /* failure */
    /*if (errno == EEXIST) {*/
    /* the file probably already existed */
    PLOG("Could not create user entry.");
    return 0;
    /*}*/

  } else {

    auto pass_hash = hash(pass);
    fprintf(file, "name=%s\npass_hash=%s\n", name, pass_hash);

  }

  fclose(file);
  return 1;
  // clang-format on

}

handle_register(state) {

  if (!csrf_validate(state)) {

    trigger_gc(1);
    goto invalid_username;

  }

  auto username = dup_alphanumeric(get_val(state, "username"));
  auto password = get_val(state, "password");

  if (!strlen(username)) { goto invalid_username; }
  if (!user_create(username, password)) { goto invalid_username; }

  cookie_set_val(state, "username", username);
  STATE->username = username;
  STATE->user_loc = loc_user(STATE->username);
  cookie_set_val(state, "logged_in", "1");
  cookie_set_val(state, "csrf", "");
  printf("success");
  return 0;

invalid_username:
  printf("<h1>Sorry, username taken!</h1>");
  trigger_gc(1);
  exit(1);
  return -1;

}

get_user_val(state, key, default_val) {

  return file_get_val(STATE->user_loc, key, default_val);

}

set_user_val(state, key, val) {

  return file_set_val(STATE->user_loc, key, val);

}

cookie_remove(state) {

  LOG("Removing cookie");
  file_delete(STATE->cookie_loc);

}

handle_login(state) {

  if (!csrf_validate(state)) {

    trigger_gc(1);
    goto user_not_found;

  }

  auto username = dup_alphanumeric(get_val(state, "username"));
  auto reset_pass = dup_alphanumeric(getenv("RESET_PASS"));
  if (reset_pass && !*STR(reset_pass)) { reset_pass = NIL; }

  LOG("Logging in...\n");
  cookie_set_val(state, "logged_in", "0");
  LOG("Login started for user %s\n", username);
  if (strlen(username) < 1) { goto user_not_found; }
  STATE->user_loc = loc_user(username);
  auto stored_pw_hash = get_user_val(state, "pass_hash", "");
  if (!stored_pw_hash) { goto user_not_found; }
  if ((!strcmp(username, ADMIN) &&
       const_time_match(state, "admin", reset_pass, CONST_MATCH_SECS)) ||
      !strcmp(hash(get_val(state, "password")), stored_pw_hash)) {

    LOG("Login successful for user %s", username);
    cookie_set_val(state, "username", username);
    cookie_set_val(state, "logged_in", "1");
    printf("success");
    return 0;

  }

user_not_found:
  LOG("Login failed for user %s. Expected pw hash %s (len %d)", username,
      stored_pw_hash, strlen(stored_pw_hash));
  printf(
#include "user_not_found.templ"
  );

error:
  cookie_remove(state);
  trigger_gc(1);
  exit(1);
  return 0;

}

run(cmd, param) {

  auto i;
  char command[1024];

  sprintf(command, cmd, param);

  LOG("Running %s\n", command);

  auto fp = popen(command, "r");
  if (!fp) {

    PLOG("run command");
    trigger_gc(1);
    exit(1);

  }

  auto ret = readline(fp);
  pclose(fp);

  if (!ret) {

    LOG("No Return\n");
    ret = "";

  } else {

    // clang-format off
    auto pos = strlen(ret);

    if (pos) {

      /* strip newline endings, pos downto 0 */

      while (pos --> 0) {

        if (STR(ret)[pos] != '\n') { break; }
        STR(ret)[pos] = '\0';

      }

    }

    /* LOG("Return was %s\n", ret); */
    // clang-format on

  }

  return ret;

}

hash(to_hash) {

  return run("echo '%s' | sha256sum", escape_4_hash(to_hash));

}

handle_reserve(state) {

  if (!csrf_validate(state)) {

    auto error = "Hackers not welcome on the web - CSRF validation failed.";
    printf(
#include <error.templ>
    );
    return 0;

  }

  auto csrf = "";

  auto admin_challenge = rand_str(ADMIN_CHALLENGE_LEN);
  auto msg_token = rand_str(MSG_TOKEN_LEN);
  LOG("admin_challenge was: %s\n", admin_challenge);
  LOG("msg_token was: %s\n", msg_token);
  auto color = get_val(state, "color");

  char msg_space[1036];
  sprintf(msg_space, MSG_DIR "%s", dup_alphanumeric(msg_token));

  auto file = file_create_atomic(msg_space);
  if (!file) {

    PLOG(msg_space);
    auto error = "Sorry, msg dispensing failed. :(";
    printf(
#include <error.templ>
    );
    return 0;

  }

  fprintf(file, "%s", color);
  fclose(file);

  auto user_msgs_old = get_user_val(state, "msgs", "");
  auto user_msgs_new =
      calloc(1, strlen(user_msgs_old) + ADMIN_CHALLENGE_LEN + 2);
  /* The msgs list gets separated with slashes for serialization. */
  sprintf(user_msgs_new, "%s/%s", user_msgs_old, msg_token);
  set_user_val(state, "msgs", user_msgs_new);

  auto admin_challenge_enc = "";
  if (IS_POST || STATE->nonce[0] < '9') {

    admin_challenge_enc = "";                                      /* TODO; */

  }

  auto own_msgs = render_own_msgs(state);
  auto msgs = render_all_msgs(state);

  printf(
#include <msg_dispenser.templ>
  );
  fflush(stdout);

  auto msg_admin_id = "";
  if IS_POST {

    auto msg_admin_id = get_val(state, "msg_admin_id");

    auto id_len = strlen(msg_admin_id);
    if (!id_len) {

      LOG("Empty msg admin response received. In case you expected an admin "
          "to "
          "access this msg, there may be a proxy messing up adminness.\n");
      return -1;

    }

    if (!strcmp(admin_challenge, msg_admin_id)) {

      LOG("An admin entered the scene!\n");
      add_priority_msg_for(STATE->username, msg_token);
      printf("Admin at the web1.0!\n");

    }

  }

}

const_time_match(state, key, str, const_match_secs) {

  struct {

    int tv_sec;
    int tv_usec;

  } tv;

  gettimeofday(&tv, NIL);
  auto start_time = tv.tv_sec * 1000000 + tv.tv_usec;

  auto i;
  auto len = strlen(str);
  auto input = get_val(state, key);
  if (!input) { return 0; }
  if (len != strlen(input)) { goto MISMATCH; }

  for (i = 0; i < len && !select(0, 0, 0, 0, &sel); i++) {

    if (STR(str)[i] != STR(input)[i]) { goto MISMATCH; }
    *sel = 1;

  }

  return 1;

MISMATCH:

  gettimeofday(&tv, NIL);
  auto end_time = tv.tv_sec * 1000000 + tv.tv_usec;
  auto sleep_time = const_match_secs * 1000000 - (end_time - start_time);
  assert(sleep_time > 0);
  LOG("Mismatch found, sleeping for %d millis\n", sleep_time);
  usleep(sleep_time);
  return 0;

}

handle_dispense(state) {

  auto admin_challenge = rand_str(ADMIN_CHALLENGE_LEN);
  auto msg_token = "";
  LOG("admin_challenge was: %s\n", admin_challenge);
  LOG("msg_token was: %s\n", msg_token);
  auto color = "";

  auto admin_challenge_enc = "";
  auto own_msgs = render_own_msgs(state);
  auto msgs = render_all_msgs(state);
  if (!msgs) { msgs = ""; }

  auto csrf = csrf_new(state);

  printf(
#include <msg_dispenser.templ>
  );

}

add_priority_msg_for(username, msg_token) {

  char priority_msg_space[1036];
  sprintf(priority_msg_space, PRIORITY_MSG_DIR "%s_%s",
          dup_alphanumeric(msg_token), username);

  auto file = file_create_atomic(priority_msg_space);
  if (!file) {

    PLOG(priority_msg_space);

  } else {

    fclose(file);

  }

}

handle_msg(state) {

  auto i;
  auto msg = get_val(state, "token");

  KV msgs = own_msg_list(state);

  for (i = 0; msgs[i]; i++) {

    if (!strcmp(msg, msgs[i])) {

      auto username = STATE->username;
      auto color = escape_4_html(get_msg_color(msg));
      printf(
#include <msg_details.templ>
      );
      return 0;

    }

  }

  LOG("User %s does not posess msg %s\n", STATE->username, msg);
  auto error = "This msg belongs to a different user. The web is secure.\n";
  printf(
#include <error.templ>
  );

}

get_msg_color(msg) {

  char msgpath[1036];
  auto color = calloc(1, 4096);
  sprintf(msgpath, MSG_DIR "%s", msg);
  LOG("Reading color from msg at %s\n", msg);
  auto file = fopen(msgpath, "r");
  if (!file) {

    PLOG("Read msg color");
    trigger_gc(1);

  }

  fread(color, 1, 4096, file);
  fclose(file);
  return color;

}

