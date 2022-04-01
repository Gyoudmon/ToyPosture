#ifndef _ZAHLEN_H
#define _ZAHLEN_H

#include "linked_list.h"

/*************************************************************************************************/
typedef struct zahlen {
    int value;
    linked_list_node_t node;
} zahlen_t;

typedef struct zahlen_env {
    linked_list_node_t* head;
    linked_list_node_t* tail;
} zahlen_env_t;

#define zahlen_entry(self) (containerof(self, zahlen_t, node))
#define zahlen_entry_value(self) zahlen_entry(self)->value

/*************************************************************************************************/
zahlen_t* zahlen_create(int value);

zahlen_env_t* zahlen_env_initialize();
void zahlen_env_push_value(zahlen_env_t* master, int value);

#endif /* _ZAHLEN_H */

