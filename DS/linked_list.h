#ifndef _LINKED_LIST_H
#define _LINKED_LIST_H

#include <stdlib.h>

/*************************************************************************************************/

/**
 * Get the offset of a member `field` in the struct `type`
 *   by supposing an instance of the `type` is located at position 0
 *   in which case addresses of members are just the same as their offsets.
 */
#define offsetof(type, field) ((size_t)&((type*)0)->field)


/**
 * Get the pointer to the container instance of `type`
 *   based on the address of the member `field` inside the target instance
 *   simply by subtracting the offset of `field` in the `type`
 *
 * NOTE that pointer arithmetic is based on the size of the type of the pointer
 *   so that explicitly casting the instance pointer to `char *` is required
 *   before substracting the offset
 */
#define containerof(addr, type, field) (type*)((char*)(addr) - offsetof(type, field))
    
/*************************************************************************************************/
typedef struct singly_list_node {
    struct singly_list_node *next;
} singly_list_node_t;

typedef long long int (*singly_list_entry_datum_f)(singly_list_node_t*);

/*************************************************************************************************/
static inline void singly_list_initialize(singly_list_node_t *self) {
    self->next = NULL;
}

static inline void singly_list_add_between(singly_list_node_t* self, singly_list_node_t* prev, singly_list_node_t* next) {
    prev->next = self;
    self->next = next;
}

static inline void singly_list_remove_between(singly_list_node_t* prev, singly_list_node_t* next) {
    prev->next = next;
}

#define singly_list_foreach(self, head) for (singly_list_node_t* self = (head); self != NULL; self = self->next)

/*************************************************************************************************/
long long int singly_list_size(singly_list_node_t* head, singly_list_node_t* nil);
singly_list_node_t* singly_list_split_half(singly_list_node_t* head, singly_list_node_t* nil);
singly_list_node_t* singly_list_merge_sort(singly_list_node_t* head, singly_list_node_t* nil, singly_list_entry_datum_f entry_datum);

#endif /* _LINKED_LIST_H */

