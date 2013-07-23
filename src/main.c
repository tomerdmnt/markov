
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <time.h>
#include <getopt.h>
#include "hash.h"
#include "list.h"

// opts
enum {
    NPREFIX = 2
};

static int hash_size = 10000;
static int maxwords = 50;

char NOWORD[] = "\n";
hash_s *states;
hash_s *words;

typedef struct state_s {
    char *prefix[NPREFIX];
    list_s *suffix_l;
} state_s;

static size_t prefix_hash(void *data) {
    char **prefix = (char **)data;
    size_t h = 0;

    for (int i = 0; i < NPREFIX; i++) {
        char *str = prefix[i];
        for (; *str; str++) 
            h = 101 * h + *str;
    }

    return h;
}

static bool prefix_cmp(void *p1, void *p2) {
    char **pre1 = (char **)p1;
    char **pre2 = (char **)p2;

    for (int i = 0; i < NPREFIX; i++)
        if (strcmp(pre1[i], pre2[i])) return false;

    return true;
}

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

        state_s *st = hash_get(states, prefix);
        if (!st) {
            st = malloc(sizeof(state_s));
            memcpy(st->prefix, prefix, sizeof(prefix[0])*NPREFIX);
            st->suffix_l = list_new();
            hash_put(states, st->prefix, st);
        }
        list_add(st->suffix_l, word);
        // move words in prefix to the right
        memmove(prefix, prefix+1, (NPREFIX-1)*sizeof(prefix[0]));
        prefix[1] = word;
    }
}

static void generate(int maxwords) {
    char *prefix[NPREFIX] = {};
    for (int i = 0; i < NPREFIX; i++) prefix[i] = NOWORD;

    for (int i = 0; i < maxwords; i++) {
        int nmatch = 0;
        char *word;
        state_s *st = hash_get(states, prefix);

        if (!st) break;
        // select the suffix randomly
        for (list_node_s *n = st->suffix_l->head->next; n; n = n->next) {
            srandom(time(NULL));
            if (random() % ++nmatch == 0) word = n->data;
        }

        printf("%s\n", word);
        // move words in prefix to the right
        memmove(prefix, prefix+1, (NPREFIX-1)*sizeof(prefix[0]));
        prefix[1] = word;
    }
}

static void usage() {
    printf("  Usage: markov [options]\n");
    printf("    reads from stdin and writes a markov chain text to stdout\n\n");
    printf("  Options:\n\n");
    printf("    -s, --hash-size\t\tSets hash table size [10000]\n");
    printf("    -w, --max-words\t\tSets The maximum words [50]\n");
    printf("    -p, --prefix-words\t\tSets the prefix [2]\n");
    printf("    -h, --help\t\t\tPrints this help message\n\n");

    exit(1);
}

static struct option longopts[] = {
    { "hash-size",      required_argument,              NULL,           's' },
    { "max-words",      required_argument,              NULL,           'w' },
    { "prefix-words",   required_argument,              NULL,           'p' },
    { "help",           no_argument,                    NULL,           'h' },
    { NULL,             0,                              NULL,           0 }
};

static void parse_opts(int argc, char **argv) {
    char ch;
    while ((ch = getopt_long(argc, argv, "s:w:p:h", longopts, NULL)) != -1)
    {
            switch (ch)
            {
                case 's':
                    hash_size = atoi(optarg);
                    break;
                case 'w':
                    maxwords = atoi(optarg);
                    break;
                case 'p':
                    //NPREFIX = &optarg;
                    break;
                case 'h':
                case '?':
                default:
                    usage();
                    break;
            }
    }
}

#ifndef NO_MAIN
int main(int argc, char **argv) {
    parse_opts(argc, argv);
    char *prefix[NPREFIX] = {};
    for (int i = 0; i < NPREFIX; i++) prefix[i] = NOWORD;
    states = hash_new(.size = hash_size, .hash_fn=prefix_hash, .keys_cmp=prefix_cmp);
    words = hash_new(.size = hash_size);

    build(prefix, stdin);
    generate(maxwords);

    hash_free(states);
    hash_free(words);
}
#endif

