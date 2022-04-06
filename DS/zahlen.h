#ifndef _ZAHLEN_H
#define _ZAHLEN_H

#include "linked_list.h"

/*************************************************************************************************/
typedef struct zahlen {
    long long int datum;
    singly_list_node_t node;
} zahlen_t;

typedef struct zahlen_env {
    singly_list_node_t* head;
    singly_list_node_t* tail;
} zahlen_env_t;

#define zahlen_entry(self) (containerof(self, zahlen_t, node))
#define zahlen_entry_datum(self) zahlen_entry(self)->datum

long long int zahlen_datum_from_node(singly_list_node_t* self);

/*************************************************************************************************/
zahlen_env_t* zahlen_env_construct();
void zahlen_env_destruct(zahlen_env_t* master);

void zahlen_env_push_datum(zahlen_env_t* master, long long int datum);
int zahlen_env_pop_datum(zahlen_env_t* master, long long int* datum);
void zahlen_env_sort(zahlen_env_t* master);

long long int zahlen_env_size(zahlen_env_t* master);
long long int zahlen_env_ref(zahlen_env_t* master, int idx, long long int default_value);

void zahlen_env_sum(zahlen_env_t* master, long long int *odd_sum, long long int* even_sum);

#endif /* _ZAHLEN_H */

