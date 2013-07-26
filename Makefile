P = markov
SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)
CFLAGS = -g -Wall -O3 -Wno-initializer-overrides
LDLIBS = 
PREFIX=/usr/local
CC = clang

$(P): $(OBJ)
	$(CC) $(CFLAGS) $(LDLIBS) $(OBJ) -o markov

test:
	$(CC) -DNO_MAIN $(CFLAGS) -Wno-unused-function $(LDLIBS) $(SRC) test.c -o test
	time ./test | column -t -s '|'

tags:
	ctags -R *

install: $(P)
	install $(P) $(PREFIX)/bin

uninstall:
	rm $(PREFIX)/bin/$(P)

clean:
	rm -f $(P) $(OBJ)

.PHONY: clean install uninstall test tags
