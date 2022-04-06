#include "../linked_list.h"
#include "../zahlen.h"

/**
 * WARNING
 * Don't delete this file even if there is no APIs inside
 *   as it is a stub for the building system
 **/


/*************************************************************************************************/
__lambda__ void zahlen_env_to_array(zahlen_env_t* master, long long int* dest, int size) {
    int idx = 0;

    singly_list_foreach(self, master->head) {
        if (idx < size) {
           dest[idx++] = zahlen_entry_datum(self); 
        } else {
            break;
        }    
    }
}

