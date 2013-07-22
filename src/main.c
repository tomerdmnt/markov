
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include "hash.h"
#include "list.h"

enum {
    NPREFIX = 2
};

hash_s *states;
hash_s *words;

typedef struct state_s {
    char *prefix[NPREFIX];
    list_s *suffix_l;
} state_s;

static void build(char *prefix[NPREFIX], FILE *f) {
    char buf[100] = {};
    char fmt[10];

    sprintf(fmt, "%%%lus", sizeof(buf)-1);
    while (fscanf(stdin, fmt, buf) != EOF) {
        char *word;
        if (!(word = hash_get(words, buf))) {
            word = strdup(buf);
            hash_put(words, word, word);
        }
        state_s *st = malloc(sizeof(state_s));
        memcpy(st->prefix, prefix, sizeof(prefix[0])*NPREFIX);
        st->suffix_l = list_new();
        // move words in prefix to the right
        memmove(prefix, prefix+1, (NPREFIX-1)*sizeof(prefix[0]));
        prefix[1] = word;
    }
}

#ifndef NO_MAIN
int main(int argc, char **argv) {
    char NOWORD[] = "\n";
    states = hash_new(10000);
    words = hash_new(10000);

    prefix[0] = NOWORD; prefix[1] = NOWORD;

    hash_free(states);
    hash_free(words);
}
#endif

