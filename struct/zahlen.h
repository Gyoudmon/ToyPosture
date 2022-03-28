#ifndef _ZAHLEN_H
#define _ZAHLEN_H

#include "linked_list.h"

typedef struct zahlen {
    int value;
    linked_list_node_t node;
} zahlen_t;

zahlen_t* zahlen_create(int value);

#endif /* _ZAHLEN_H */

