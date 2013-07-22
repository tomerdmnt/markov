
#include "list.h"

static list_node_s *list_node_new(void *data) {
    list_node_s *out = malloc(sizeof(list_node_s));
    *out = (list_node_s){ .data = data };
    return out;
}

static void list_node_free(list_node_s *in) {
    free(in);
}

list_s *list_new() {
    list_s *out = malloc(sizeof(list_s));
    list_node_s *head = list_node_new(NULL);
    *out = (list_s){ .head = head };
    return out;
}

void list_free(list_s *in) {
    for (list_node_s *n = in->head; n;) {
        list_node_s *tmp = n;
        n = n->next;
        list_node_free(tmp);
    }
    free(in);
}

void list_add(list_s *in, void *data) {
    list_node_s *n;
    list_node_s *new = list_node_new(data);

    for(n = in->head; n->next; n = n->next);
    n->next = new;
}
