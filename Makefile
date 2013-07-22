P = markov
SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)
CFLAGS = -g -Wall -O0
LDLIBS = 
PREFIX=/usr/local
CC = clang

$(P): $(OBJ)
	$(CC) $(CFLAGS) $(LDLIBS) $(OBJ) -o markov

test:
	$(CC) -DNO_MAIN $(CFLAGS) $(LDLIBS) $(SRC) test.c -o test
	time ./test | column -t -s '|'

install: $(P)
	install $(P) $(PREFIX)/bin

uninstall:
	rm $(PREFIX)/bin/$(P)

clean:
	rm -f $(P) $(OBJ)

.PHONY: clean install uninstall test
