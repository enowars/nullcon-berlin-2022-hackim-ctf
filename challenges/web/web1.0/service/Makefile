CC=gcc
# -DNO_ALARM -DNO_CSRF -O0
override CFLAGS += -O3 -g -m32 -fno-omit-frame-pointer -std=gnu89 -D_FORTIFY_SOURCE=2 -Wformat -Wno-implicit-function-declaration -Wno-format-security -Wl,-z,relro -Wl,-z,now -fstack-protector-strong -fsanitize=address -Wno-deprecated-declarations -Wno-int-conversion -Wno-format -Wno-builtin-declaration-mismatch
#override CFLAGS += -O3 -g -fno-omit-frame-pointer -std=gnu89 -D_FORTIFY_SOURCE=2 -Wformat -Wno-implicit-function-declaration -Wno-format-security -Wl,-z,relro -Wl,-z,now -fstack-protector-strong -fsanitize=address -Wno-deprecated-declarations -Wno-int-conversion

#override CFLAGS += -DNO_ALARM

ODIR=./src
TDIR=./templates
SRCDIR=./src
IDIR=./src
CGI_DIR=./cgi-bin

LIBS=-lm

all: web1.0

$(TDIR)/%.templ: $(TDIR)/%.templ.html
	./to_templ.py $<

$(ODIR)/%.o: $(SRCDIR)/%.c templates
	$(CC) -c -o $@ $< $(CFLAGS) -I$(TDIR)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ cgi-bin/web1.0 $(TDIR)/*.templ

templates: $(TDIR)/msg_details.templ $(TDIR)/msg.templ $(TDIR)/error.templ $(TDIR)/body_index.templ $(TDIR)/head.templ $(TDIR)/msg_dispenser.templ $(TDIR)/user_not_found.templ

web1.0: $(ODIR)/web1.0.o
	$(CC) -o $(CGI_DIR)/$@ $^ $(CFLAGS) $(LIBS)

../data:
	mkdir -p ../data/cookies ../data/users ../data/msgs ../data/priority_msgs

run: web1.0 ../data
	./web1.0.py

test_rand: ../data
	make clean
	make web1.0 'CFLAGS=-DTEST -DTEST_RAND'
	./cgi-bin/web1.0
	make clean
test_time: ../data
	make clean
	make web1.0 'CFLAGS=-DTEST -DTEST_TIME'
	./cgi-bin/web1.0
	make clean
test_query_parser: ../data
	make clean
	make web1.0 'CFLAGS=-DTEST -DTEST_QUERY_PARSER'
	echo "test=success" | ./cgi-bin/web1.0
	make clean
test_alpha: ../data
	make clean
	make web1.0 'CFLAGS=-DTEST -DTEST_ALPHA -w'
	./cgi-bin/web1.0
	make clean
test_val: ../data
	make clean
	make web1.0 'CFLAGS=-DTEST -DTEST_VAL -w'
	./cgi-bin/web1.0
	make clean
test_readline: ../data
	make clean
	make web1.0 'CFLAGS=-DTEST -DTEST_READLINE -w'
	echo "line" | ./cgi-bin/web1.0
	make clean
test_hash: ../data
	make clean
	make web1.0 'CFLAGS=-DTEST -DTEST_HASH -w'
	./cgi-bin/web1.0
	make clean
test_ini_files: ../data
	make clean
	make web1.0 'CFLAGS=-DTEST -DTEST_INI_FILES -w'
	./cgi-bin/web1.0
	make clean
test_escape: ../data
	make clean
	make web1.0 'CFLAGS=-DTEST -DTEST_ESCAPE -w'
	./cgi-bin/web1.0
	make clean
test_cookie_parser: ../data
	make clean
	make web1.0 'CFLAGS=-DTEST -DTEST_COOKIE_PARSER -w'
	./cgi-bin/web1.0
	make clean
test_msg_enc: ../data
	make clean
	make web1.0 'CFLAGS=-DTEST -DTEST_MSG_ENC -w'
	./cgi-bin/web1.0
	make clean

tests: test_msg_enc test_cookie_parser test_escape test_ini_files test_rand test_query_parser test_alpha test_val test_readline test_hash
	@echo ""
	@echo "[*] All tests done"

fmt:
	./.custom-format.py -i $(SRCDIR)/*.c
	./.custom-format.py -i $(SRCDIR)/*.h
	python3 -m black .
