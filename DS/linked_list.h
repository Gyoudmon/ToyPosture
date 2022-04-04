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
typedef struct linked_list_node {
    struct linked_list_node *next;
} linked_list_node_t;

/*************************************************************************************************/
static inline void linked_list_initialize(linked_list_node_t *self) {
    self->next = NULL;
}

static inline void linked_list_add_between(linked_list_node_t* self, linked_list_node_t* prev, linked_list_node_t* next) {
    prev->next = self;
    self->next = next;
}

static inline void linked_list_remove_between(linked_list_node_t* prev, linked_list_node_t* next) {
    prev->next = next;
}

#define linked_list_foreach(self, head) for (linked_list_node_t* self = (head); self != NULL; self = self->next)

#endif /* _LINKED_LIST_H */

