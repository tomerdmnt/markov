
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#ifndef KEYVAL_H
#define KEYVAL_H

typedef struct keyval_s {
    char *key;
    void *val;
} keyval_s;

keyval_s *keyval_new(char *, void *);
void keyval_free(keyval_s *);
bool keyval_matches(keyval_s *, char *);

#endif
