
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "list.h"
#include "keyval.h"

#ifndef HASH_H
#define HASH_H

static uint32_t hash(char const *str) {
    uint32_t h = 0;
    for (; *str; str++)
        h = 101 * h + *str;
    return h;
}

typedef struct hash_bucket_s {
    list_s *items;
} hash_bucket_s;

typedef struct hash_s {
    hash_bucket_s *buckets;
    uint32_t size;
} hash_s;

hash_s *hash_new(uint32_t);
hash_s *hash_copy(hash_s *);
void hash_free(hash_s *);
void hash_put(hash_s *, char *, void *);
void *hash_get(hash_s *, char *);

#endif
