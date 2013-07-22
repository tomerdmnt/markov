
#include <stdio.h>
#include <assert.h>
#include "src/hash.h"
#include "src/list.h"
#include "src/keyval.h"

#define run_test(desc, name, ...) {     \
    printf(desc "|");                 \
    test_ ## name(__VA_ARGS__);         \
    printf("OK\n");                     \
}

void test_hash_func() {
    uint32_t h = hash("hello world");
    assert(h == 1232049484);
}

void test_list() {
    list_s *l = list_new();
    list_add(l, "foo");
    list_add(l, "bar");
    assert(!strcmp((char *)l->head->next->data, "foo"));
    assert(!strcmp((char *)l->head->next->next->data, "bar"));
    list_free(l);
}

void test_keyval() {
    keyval_s *kv = keyval_new("foo", "bar");
    assert(!strcmp(kv->key, "foo"));
    assert(!strcmp((char *)kv->val, "bar"));
    assert(keyval_matches(kv, "foo"));
}

void test_hash(uint32_t size) {
    hash_s *h = hash_new(size);
    hash_put(h, "foo", "foobar");
    hash_put(h, "bar", "barfoo");
    char *foobar = (char *)hash_get(h, "foo");
    assert(foobar != NULL);
    assert(!strcmp(foobar, "foobar"));
    char *barfoo = (char *)hash_get(h, "bar");
    assert(barfoo != NULL);
    assert(!strcmp(barfoo, "barfoo"));
    hash_free(h);
}

int main(int argc, char **argv) {
    run_test("hash function", hash_func);
    run_test("list", list);
    run_test("keyval", keyval);
    run_test("hash small", hash, 1);
    run_test("hash huge", hash, 100000);
}

