
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <time.h>
#include <getopt.h>
#include "hash.h"
#include "list.h"

// opts
static int nprefix = 2;
static int hash_size = 10000;
static int maxwords = 50;
static char *delim = " ";

static char NOWORD[] = "\n";
hash_s *states;
hash_s *words;

typedef struct state_s {
    char **prefix;
    list_s *suffix_l;
} state_s;

static size_t prefix_hash(void *data) {
    char **prefix = (char **)data;
    size_t h = 0;

    for (int i = 0; i < nprefix; i++) {
        char *str = prefix[i];
        for (; *str; str++) 
            h = 101 * h + *str;
    }

    return h;
}

static bool prefix_cmp(void *p1, void *p2) {
    char **pre1 = (char **)p1;
    char **pre2 = (char **)p2;

    for (int i = 0; i < nprefix; i++)
        if (strcmp(pre1[i], pre2[i])) return false;

    return true;
}

static void build(char *prefix[nprefix], FILE *f) {
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
            st->prefix = malloc(sizeof(prefix[0])*nprefix);
            memcpy(st->prefix, prefix, sizeof(prefix[0])*nprefix);
            st->suffix_l = list_new();
            hash_put(states, st->prefix, st);
        }

        list_push(st->suffix_l, word);
        // move words in prefix to the left 
        memmove(prefix, prefix+1, (nprefix-1)*sizeof(prefix[0]));
        prefix[nprefix-1] = word;
    }
}

static void generate(int maxwords) {
    char *prefix[nprefix];
    for (int i = 0; i < nprefix; i++) prefix[i] = NOWORD;

    // prepare output format
    char fmt[3+strlen(delim)];
    strcpy(fmt, "%s");
    strcat(fmt, delim);

    for (int i = 0; i < maxwords; i++) {
        state_s *st = hash_get(states, prefix);

        // DEBUG:
        //printf("checking prefix: ");
        //for (int i = 0; i < nprefix; i++) printf("%s,", prefix[i]);
        //printf("\n");

        if (!st) break;
        // DEBUG
        //printf("checking words:\n");
        // select the suffix randomly
        srandom(time(NULL));
        size_t index = random() % st->suffix_l->len;
        char *word = list_get(st->suffix_l, index);
        // DEBUG
        //printf("random index: %zd of %zd", index, st->suffix_l->len);
        //printf("\n");

        printf(fmt, word);
        // move words in prefix to the left
        memmove(prefix, prefix+1, (nprefix-1)*sizeof(prefix[0]));
        prefix[nprefix-1] = word;
        // DEBUG
        //printf("\n\n");
    }
}

static void usage() {
    printf("  Usage: markov [options]\n\n");
    printf("    reads from stdin and writes a markov chain text to stdout\n\n");
    printf("  Options:\n\n");
    printf("    -p, --prefix-words\t\tNumber of words in prefix [2]\n");
    printf("    -w, --max-words\t\tMaximum number of words to output [50]\n");
    printf("    -d, --delimiter\t\tDelimiter between words in output [' ']\n");
    printf("    -s, --hash-size\t\tHash tables sizes [10000]\n");
    printf("    -h, --help\t\t\tPrints this help message\n\n");

    exit(1);
}

static struct option longopts[] = {
    { "hash-size",      required_argument,              NULL,           's' },
    { "max-words",      required_argument,              NULL,           'w' },
    { "prefix-words",   required_argument,              NULL,           'p' },
    { "delimiter",      required_argument,              NULL,           'd' },
    { "help",           no_argument,                    NULL,           'h' },
    { NULL,             0,                              NULL,           0 }
};

static void parse_opts(int argc, char **argv) {
    char ch;
    while ((ch = getopt_long(argc, argv, "s:w:p:d:h", longopts, NULL)) != -1)
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
                nprefix = atoi(optarg);
                break;
            case 'd':
                delim = strdup(optarg);
                if (!strcmp(delim, "\\n")) delim = "\n";
                if (!strcmp(delim, "\\t")) delim = "\t";
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
    char *prefix[nprefix];
    for (int i = 0; i < nprefix; i++) prefix[i] = NOWORD;
    states = hash_new(.size = hash_size, .hash_fn=prefix_hash, .keys_cmp=prefix_cmp);
    words = hash_new(.size = hash_size);

    build(prefix, stdin);
    generate(maxwords);
    printf("\n");

    hash_free(states);
    hash_free(words);
}
#endif

