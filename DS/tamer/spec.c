#include <stdio.h>

#include "../zahlen.h"

/*************************************************************************************************/
__lambda__ int zahlen_env_count(zahlen_env_t* master) {
    int n = 0;

    for (linked_list_node_t* self = master->head; self != NULL; self = self->next) {
        n++;
    }

    return n;
}

__lambda__ int zahlen_env_ref(zahlen_env_t* master, int idx, int default_value) {
    int datum = default_value;

    for (linked_list_node_t* self = master->head; self != NULL; self = self->next) {
        if (idx == 0) {
            datum = zahlen_entry_value(self);

            break;
        }

        idx--;
    }

    return datum;
}

