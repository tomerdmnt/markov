
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "list.h"
#include "keyval.h"

#ifndef HASH_H
#define HASH_H

typedef size_t (*hash_t)(void *);
size_t char_hash(void *data);

typedef struct hash_bucket_s {
    list_s *items;
} hash_bucket_s;

typedef struct hash_s {
    size_t size;
    hash_bucket_s *buckets;
    hash_t hash_fn;
    keys_cmp_t keys_cmp;
} hash_s;

#define hash_new(...) hash_new_(&(hash_s)	\
		{.size = 1024, __VA_ARGS__});

hash_s *hash_new_(hash_s *in);
hash_s *hash_copy(hash_s *);
void hash_free(hash_s *);
void hash_put(hash_s *, void *, void *);
void *hash_get(hash_s *, void *);

#endif
