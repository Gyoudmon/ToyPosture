#include <stdlib.h>

#include "zahlen.h"
#include "linked_list.h"

/** NOTE
 * The token `__lambda__` should be defined as `declspec(export)` to satisfy the MSVC 
 */

/*************************************************************************************************/
__lambda__ zahlen_t* zahlen_create(int value) {
    size_t size = sizeof(zahlen_t);
    zahlen_t* self = NULL;

    self = (zahlen_t*)malloc(size);
    self->value = value;

    linked_list_initialize(&self->node);

    return self;
}

/*************************************************************************************************/
__lambda__ zahlen_env_t* zahlen_env_initialize() {
    size_t size = sizeof(zahlen_env_t);
    zahlen_env_t* self = NULL;

    self = (zahlen_env_t*)malloc(size);
    self->head = NULL;
    self->tail = NULL;

    return self;
}

__lambda__ void zahlen_env_push_value(zahlen_env_t* master, int value) {
    zahlen_t* self = NULL;

    self = zahlen_create(value);

    if (master->head == NULL) {
        master->head = &self->node;
        master->tail = &self->node;
    } else {
        linked_list_add_between(&self->node, master->tail, NULL);
        master->tail = master->tail->next;
    }
}

