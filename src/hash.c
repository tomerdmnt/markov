
#include "hash.h"

size_t char_hash(void *data) {
    char *str = (char *)data;
    size_t h = 0;
    for (; *str; str++)
        h = 101 * h + *str;
    return h;
}

hash_s *hash_new_(hash_s *in) {
    hash_s *out = malloc(sizeof(hash_s));
    *out = *in;
    hash_bucket_s *buckets = calloc(out->size, sizeof(hash_bucket_s));
    out->buckets = buckets;
    if (!out->hash_fn) out->hash_fn = char_hash;
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

void hash_put(hash_s *in, void *key, void *data) {
    size_t h = in->hash_fn(key) % in->size;
    hash_bucket_s *bucket = &in->buckets[h];
    // create the items list if necessary
    if (!bucket->items) 
        bucket->items = list_new();

    // create the keyval and put it in the bucket
    keyval_s *kv = keyval_new(key, data, .cmp=in->keys_cmp);
    list_push(bucket->items, kv);
}

void *hash_get(hash_s *in, void *key) {
    size_t h = in->hash_fn(key) % in->size;
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

