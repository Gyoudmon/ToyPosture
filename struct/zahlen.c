#include <stdlib.h>

#include "zahlen.h"
#include "linked_list.h"

/*************************************************************************************************/
__lambda__ zahlen_t* zahlen_create(int value) {
    size_t size = sizeof(zahlen_t);
    zahlen_t* self = NULL;

    self = (zahlen_t*)malloc(size);
    self->value = value;

    linked_list_initialize(&self->node);

    return self;
}

