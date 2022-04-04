#include <stdlib.h>

#include "zahlen.h"
#include "linked_list.h"

/** NOTE
 * The token `__lambda__` should be defined as `__declspec(export)` to satisfy the MSVC 
 */

/*************************************************************************************************/
__lambda__ zahlen_t* zahlen_create(long long int datum) {
    size_t size = sizeof(zahlen_t);
    zahlen_t* self = NULL;

    self = (zahlen_t*)malloc(size);
    self->datum = datum;

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

__lambda__ void zahlen_env_push_datum(zahlen_env_t* master, long long int datum) {
    zahlen_t* self = NULL;

    self = zahlen_create(datum);

    if (master->head == NULL) {
        master->head = &self->node;
        master->tail = &self->node;
    } else {
        linked_list_add_between(&self->node, master->tail, NULL);
        master->tail = master->tail->next;
    }
}

__lambda__ void zahlen_env_sum(zahlen_env_t* master, long long int* odd_sum, long long int* even_sum) {
    long long int o_sum = 0;
    long long int e_sum = 0;

    linked_list_foreach(self, master->head) {
        long long int datum =  zahlen_entry_datum(self);

        if ((datum & 0b01) == 1) {
            o_sum += datum;
        } else {
            e_sum += datum;
        }
    }

    if (odd_sum != NULL) {
        (*odd_sum) = o_sum;
    }

    if (even_sum != NULL) {
        (*even_sum) = e_sum;
    }

}

