
#include "keyval.h"

keyval_s *keyval_new(char *key, void *val) {
    keyval_s *out = malloc(sizeof(keyval_s));
    *out = (keyval_s){ .key = key, .val = val};
    return out;
}

void keyval_free(keyval_s *in) {
    free(in);
}

bool keyval_matches(keyval_s *in, char *key) {
    return !strcmp(in->key, key);
}
