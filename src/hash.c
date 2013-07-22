
#include "hash.h"

hash_s *hash_new(uint32_t size) {
    hash_s *out = malloc(sizeof(hash_s));
    hash_bucket_s *buckets = calloc(size, sizeof(hash_bucket_s));
    *out = (hash_s){.buckets = buckets, .size = size};
    return out;
}

hash_s *hash_copy(hash_s *in) {
    hash_s *out = malloc(sizeof(hash_s));
    *out = *in;
    out->buckets = calloc(in->size, sizeof(hash_bucket_s));
    out->buckets = memcpy(out->buckets, in->buckets, 
                        out->size * sizeof(hash_bucket_s));
    return out;
}

void hash_bucket_free(hash_bucket_s *in) {
}

void hash_free(hash_s *in) {
    // free all lists
    for (int i = 0; i < in->size; i++) {
        list_s *items = in->buckets[i].items;
        if (items) list_free(items);
    }
    free(in->buckets);
    free(in);
}

void hash_put(hash_s *in, char *key, void *data) {
    uint32_t h = hash(key) % in->size;
    hash_bucket_s *bucket = &in->buckets[h];
    // create the items list if necessary
    if (!bucket->items) 
        bucket->items = list_new();

    // create the keyval and put it in the bucket
    keyval_s *kv = keyval_new(key, data);
    list_add(bucket->items, kv);
}

void *hash_get(hash_s *in, char *key) {
    uint32_t h = hash(key) % in->size;
    hash_bucket_s *bucket = &in->buckets[h];
    if (!bucket->items) 
        return NULL;

    // find the item
    for (list_node_s *n = bucket->items->head->next; n; n = n->next) {
        keyval_s *kv = (keyval_s *)n->data;
        if (keyval_matches(kv, key))
            return kv->val;
    }

    return NULL;
}

