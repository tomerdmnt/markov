
#include "keyval.h"

static bool default_cmp(void *k1, void *k2) {
    char *s1 = k1;
    char *s2 = k2;

    return !strcmp(s1, s2);
}

keyval_s *keyval_new_(keyval_s *in) {
    keyval_s *out = malloc(sizeof(keyval_s));
    *out = *in;
    if (!out->cmp) out->cmp = default_cmp;
    return out;
}

void keyval_free(keyval_s *in) {
    free(in);
}

bool keyval_matches(keyval_s *in, void *key) {
    return in->cmp(in->key, key);
}
