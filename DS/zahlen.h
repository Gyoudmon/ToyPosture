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

__lambda__ long long int zahlen_datum_from_node(singly_list_node_t* self);

/*************************************************************************************************/
__lambda__ zahlen_env_t* zahlen_env_construct();
__lambda__ void zahlen_env_destruct(zahlen_env_t* master);

__lambda__ void zahlen_env_push_datum(zahlen_env_t* master, long long int datum);
__lambda__ int zahlen_env_pop_datum(zahlen_env_t* master, long long int* datum);
__lambda__ void zahlen_env_sort(zahlen_env_t* master);

__lambda__ long long int zahlen_env_size(zahlen_env_t* master);
__lambda__ long long int zahlen_env_ref(zahlen_env_t* master, int idx, long long int default_value);

__lambda__ void zahlen_env_sum(zahlen_env_t* master, long long int *odd_sum, long long int* even_sum);

#endif /* _ZAHLEN_H */

