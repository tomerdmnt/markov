
#include <stdlib.h>

#ifndef LIST_H
#define LIST_H

typedef struct list_node_s {
    struct list_node_s *next;
    void *data;
} list_node_s;

typedef struct list_s {
    list_node_s *head;
} list_s;

list_s *list_new();
void list_free(list_s *);
void list_add(list_s *, void *);

#endif
