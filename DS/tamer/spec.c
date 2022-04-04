#include <stdio.h>

#include "../zahlen.h"

/*************************************************************************************************/
__lambda__ long long int zahlen_env_count(zahlen_env_t* master) {
    long long int n = 0;

    linked_list_foreach(self, master->head) {
        n++;
    }

    return n;
}

__lambda__ long long int zahlen_env_ref(zahlen_env_t* master, int idx, long long int default_value) {
    long long int datum = default_value;

    linked_list_foreach(self, master->head) {
        if (idx == 0) {
            datum = zahlen_entry_datum(self);

            break;
        }

        idx--;
    }

    return datum;
}

