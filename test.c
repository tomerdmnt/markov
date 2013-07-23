
#include <stdio.h>
#include <assert.h>
#include "src/hash.h"
#include "src/list.h"
#include "src/keyval.h"
#include "src/main.c"

#define run_test(desc, name, ...) {     \
    printf(desc "|");                 \
    test_ ## name(__VA_ARGS__);         \
    printf("OK\n");                     \
}

void test_hash_func() {
    unsigned int h = char_hash("hello world");
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
    hash_s *h = hash_new(.size=size);
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

void test_prefix_hash() {
    char *prefix[2] = { "hello ", "world" };
    unsigned int h = prefix_hash(prefix);
    assert(h == 1232049484);
}

void test_prefix_cmp() {
    char *prefix1[2] = { "foo", "bar" };
    char *prefix2[2] = { "foo", "bar" };
    
    assert(prefix_cmp(prefix1, prefix2));
    prefix2[0] = "bar";
    prefix2[1] = "foo";
    assert(!prefix_cmp(prefix1, prefix2));
}

void test_prefix() {
    hash_s *h = hash_new(.size=10, .keys_cmp=prefix_cmp, .hash_fn=prefix_hash);
    char *prefix1[2] = { "foo", "bar" };
    char *prefix2[2] = { "bar", "foo" };
    hash_put(h, prefix1, "foobar");
    hash_put(h, prefix2, "barfoo");
    assert(!strcmp(hash_get(h, (char *[2]){ "foo", "bar"}), "foobar"));
    assert(!strcmp(hash_get(h, (char *[2]){ "bar", "foo"}), "barfoo"));
    hash_free(h);
}

int main(int argc, char **argv) {
    run_test("hash function", hash_func);
    run_test("list", list);
    run_test("keyval", keyval);
    run_test("hash small", hash, 1);
    run_test("hash huge", hash, 100000);
    run_test("prefix cmp", prefix_cmp);
    run_test("prefix hash", prefix_hash);
    run_test("prefix", prefix);
}

