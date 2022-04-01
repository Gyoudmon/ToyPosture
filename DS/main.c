#include <stdio.h>
#include <stdlib.h>

#include "linked_list.h"
#include "zahlen.h"

/*************************************************************************************************/
static int read_integer_from_line() {
    char* line = NULL;
    size_t size;
    int n;

    getline(&line, &size, stdin);
    n = atoi(line);
    free(line);

    return n;
}

static inline int read_choice() {
    return read_integer_from_line();
}

/*************************************************************************************************/
static zahlen_env_t* zahlen_env_construct(int eof) {
    zahlen_env_t* master = zahlen_env_initialize();
    int n = 0;

    while (1) {
        n = read_integer_from_line();

        if (n == eof) {
            break;
        } else {
            zahlen_env_push_value(master, n);
        }
    }

    return master;
}

static void zahlen_env_display(zahlen_env_t* master, int col_size) {
    int col_idx = 0;

    for (linked_list_node_t* self = master->head; self != NULL; self = self->next) {
        printf("%d", zahlen_entry_value(self));
        col_idx++;

        if (col_idx >= col_size) {
            printf("\n");
            col_idx = 0;
        } else {
            printf(" ");
        }
    }

    if (col_idx > 0) {
        printf("\n");
    }
}

/*************************************************************************************************/
int main(int argc, char* argv[]) {
    zahlen_env_t* master = NULL;
    int is_in_repl = 1;

    while (is_in_repl) {
        printf("1. construct an unordered linked list with integer values\n");
        printf("2. display all integers in the linked list\n");
        printf("3. sort the linked list in ascending order\n");
        printf("4. sum all odd and even integers in the linked list separately\n");
        printf("5. destruct the linked list\n");
        printf("0. Exit\n");

        switch (read_choice()) {
            case 0: is_in_repl = 0; break;
            case 1: {
                master = zahlen_env_construct(-1);
            }; break;
            case 2: {
                if (master != NULL) {
                    zahlen_env_display(master, 8);
                }
            }; break;
        }
    }

    return 0;
}

