/* run tests using
    make CFLAGS='-DTEST -DTEST_RAND'
*/
#if defined(TEST)
int tests() {

  #if defined(TEST_RAND)
    #warning Running TEST_RAND

  printf("testing strlen of rand_str(16)\n");
  assert(strlen(rand_str(16)) == 16);
  printf("testing alphanumericity of rand_str(16)\n");
  auto rand = rand_str(1);
  assert(IS_ALPHANUMERIC(STR(rand)[0]));
  printf("%s\n", rand_str(16));
  return 0;

  #elif defined(TEST_COOKIE_PARSER)
    #warning Running TEST_COOKIE_PARSER

  parse_cookie("cookie");
  printf(parse_cookie(COOKIE_NAME "=testcookie;"));
  assert(!strcmp(parse_cookie(COOKIE_NAME "=testcookie;"), "testcookie"));
  assert(!strcmp(parse_cookie(COOKIE_NAME "=testcookie; "), "testcookie"));
  assert(
      !strcmp(parse_cookie("test=fun; HTTPOnly; " COOKIE_NAME "=testcookie; "),
              "testcookie"));
  return 0;

  #elif defined(TEST_QUERY_PARSER)
    #warning Running TEST_QUERY_PARSER

  auto i;
  auto parseme = "web1.0=side&fun=true&you're=beautiful!&&fun=";
  printf("parsing %s\n", parseme);
  auto query = parse_query(parseme);
  KV_FOREACH(query, { printf("key: %s, val: %s\n", key, val); })
  assert(STR(parseme)[1] == ((KV)query)[0][1]);
  return 0;

  #elif defined(TEST_ALPHA)
    #warning Running TEST_ALPHA

  auto alpha = "FUN1";
  auto nonalpha1 = "%%!FUN1";
  auto nonalpha2 = "%%!";
  auto nonalpha3 = "%%!0";
  auto alpha1 = dup_alphanumeric(nonalpha1);
  printf("%s: %s", nonalpha1, alpha1);
  assert(!strcmp(alpha, alpha1));
  free(alpha1);
  return 0;

  #elif defined(TEST_VAL)
    #warning Running TEST_VAL

  system("mkdir -p " COOKIE_DIR " " USER_DIR);
  auto state = init_state(NIL, NIL, NIL);
  auto testval = get_val(state, "test");
  if (!testval) {

    printf("No val read!");
    return 0;

  }

  printf("%s\n", testval);
  printf("%s\n", get_val(state, "test"));
  return 0;

  #elif defined(TEST_READLINE)
    #warning Running TEST_READLINE

  auto body = BODY();
  if (body) {

    printf("%s\n" NL, body);

  } else {

    printf("No body read\n");

  }

  return 0;

  #elif defined(TEST_TIME)
    #warning Running TEST_TIME

  system("mkdir -p " COOKIE_DIR " " USER_DIR);
  auto state = init_state(NIL, NIL, NIL);
  printf("Testing sleepytime, starting at %d\n", time(0));
  printf("result: %d",
         const_time_match(state, "key", "test", CONST_MATCH_SECS));
  printf("Time after: %d\n", time(0));

  return 0;

  #elif defined(TEST_HASH)
    #warning Running TEST_HASH

  printf("%s" NL, hash("test"));
  return 0;

  #elif defined(TEST_INI_FILES)
    #warning Running TEST_INI_FILES

  auto testfile = "testfile.tmp";
  auto key = "testkey";
  auto val = "testval";
  auto val2 = "testval2";
  file_delete(testfile);
  auto f = file_create_atomic(testfile);
  fclose(f);
  file_set_val(testfile, key, val);

  auto read_val = file_get_val(testfile, key, "");
  assert(!strcmp(val, read_val));

  file_set_val(testfile, "some", "value");
  file_set_val(testfile, key, val2);
  file_set_val(testfile, "someother", "value");
  system("cat testfile.tmp");
  read_val = file_get_val(testfile, key, "");
  assert(strcmp(val, read_val));
  assert(!strcmp(val2, read_val));

  file_delete(testfile);

  printf("testing empty ini. Query should be empty:\n");
  fflush(stdout);
  auto file = file_create_atomic(testfile);
  assert(file);
  debug_print_query(read_ini(testfile));
  assert(!file_get_val(testfile, "test", NIL));
  remove(testfile);

  return 0;

  #elif defined(TEST_ESCAPE)
    #warning Running TEST_ESCAPE

  printf("AA 4html = %s\n", escape_4_html("AA"));
  assert(!strcmp(escape_4_html("AA"), "&#x41;&#x41;"));
  printf("AA 4py = %s\n", escape_4_py("AA"));
  assert(!strcmp(escape_4_py("AA"), "\\x41\\x41"));
  return 0;

  #else
    #error No test specified, use -DTEST -DTEST_<TESTNAME>
  #endif

}

#endif

