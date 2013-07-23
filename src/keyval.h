
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#ifndef KEYVAL_H
#define KEYVAL_H

typedef bool (*keys_cmp_t)(void *, void *);

typedef struct keyval_s {
    void *key;
    void *val;
    keys_cmp_t cmp;
} keyval_s;

#define keyval_new(key_, val_, ...) keyval_new_(&(keyval_s) \
		{ .key = key_, .val = val_, __VA_ARGS__ });

keyval_s *keyval_new_(keyval_s *);
void keyval_free(keyval_s *);
bool keyval_matches(keyval_s *, void *);

#endif
