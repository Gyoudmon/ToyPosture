#ifndef __LINKED_LIST__
#define __LINKED_LIST__

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

#endif /* __LINKED_LIST__ */

