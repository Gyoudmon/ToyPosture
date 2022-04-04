#ifndef _ZAHLEN_H
#define _ZAHLEN_H

#include "linked_list.h"

/*************************************************************************************************/
typedef struct zahlen {
    long long int datum;
    linked_list_node_t node;
} zahlen_t;

typedef struct zahlen_env {
    linked_list_node_t* head;
    linked_list_node_t* tail;
} zahlen_env_t;

#define zahlen_entry(self) (containerof(self, zahlen_t, node))
#define zahlen_entry_datum(self) zahlen_entry(self)->datum

/*************************************************************************************************/
zahlen_t* zahlen_create(long long int datum);

zahlen_env_t* zahlen_env_initialize();
void zahlen_env_push_datum(zahlen_env_t* master, long long int datum);
void zahlen_env_sum(zahlen_env_t* master, long long int *odd_sum, long long int* even_sum);

#endif /* _ZAHLEN_H */

