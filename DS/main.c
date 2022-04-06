#include <stdio.h>
#include <stdlib.h>

#include "linked_list.h"
#include "zahlen.h"
#include "reader.h"

/*************************************************************************************************/
static long long int read_fixnum_from_line(const char* prompt) {
    long long int n;
    int count;
    int okay;

    do {
        if (prompt != NULL) {
            printf("%s ", prompt);
        }

        n = read_integer(stdin, &count);
        okay = has_reached_end_of_word(stdin);

        if (!okay) {
            printf("ignored invalid char, please try again!\n");
            discard_word(stdin);
        }
    } while(!okay && !feof(stdin));

    return n;
}

static inline int read_choice(const char* prompt) {
    return (int)read_fixnum_from_line(prompt);
}

static inline void pause() {
    printf("Press ENTER to continue...\n");
    discard_this_line(stdin);
    getchar();
}

/*************************************************************************************************/
static int zahlen_env_check(const zahlen_env_t* env, const char* message) {
    int okay = 1;

    if (env == NULL) {
        printf("%s\n", message);
        okay = 0;
    }

    return okay;
}

static zahlen_env_t* zahlen_env_initialize(int sentry) {
    zahlen_env_t* master = zahlen_env_construct();
    long long int n = sentry + 1;

    printf("Please input a sort of integers separated by whitespaces, with the ending sentinel %d:\n", sentry);

    do {
        n = read_fixnum_from_line(NULL);

        if (n != sentry) {
            zahlen_env_push_datum(master, n);
        }
    } while ((n != sentry) && !feof(stdin));
    
    return master;
}

static void zahlen_env_display(zahlen_env_t* master, int col_size) {
    int col_idx = 0;

    singly_list_foreach(self, master->head) {
        printf("%lld", zahlen_entry_datum(self));
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

static void zahlen_env_sum_up(zahlen_env_t* master) {
    long long int o_sum, e_sum;

    zahlen_env_sum(master, &o_sum, &e_sum);
    printf("Sum: odd = %lld; even = %lld\n", o_sum, e_sum);
}

/*************************************************************************************************/
int main(int argc, char* argv[]) {
    const char* null_message = "You need to construct the linked list first!";
    zahlen_env_t* master = NULL;
    int is_in_repl = 1;
    int choice;

    do {
        printf("\033[2J\n"); // clear the screen
        printf("\033[H\n");  // move cursor to the left top corner of the screen

        printf("1. construct an unordered linked list with integer values\n");
        printf("2. display all integers in the linked list\n");
        printf("3. sort the linked list in ascending order\n");
        printf("4. sum all odd and even integers in the linked list separately\n");
        printf("5. destruct the linked list\n");
        printf("0. Exit\n\n");

        choice = read_choice("Please select the operation[0 - 5]:");
        printf("\033[J\n");  // clear the screen but leave the menu displayed
        
        switch (choice) {
            case 0: is_in_repl = 0; break;
            case 1: {
                printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
                if (master != NULL) {
                    zahlen_env_destruct(master);
                    printf("The existed linked list has been destructed!\n");
                }

                master = zahlen_env_initialize(-1);
                printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
            }; break;
            case 2: {
                if (zahlen_env_check(master, null_message)) {
                    printf("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n");
                    zahlen_env_display(master, 8);
                    printf("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n");
                }
            }; break;
            case 3: {
                if (zahlen_env_check(master, null_message)) {
                    printf("*****************************************************\n");
                    zahlen_env_sort(master);
                    printf("The linked list has been sorted:\n");
                    zahlen_env_display(master, 8);
                    printf("*****************************************************\n");
                }
            }; break;
            case 4: {
                if (zahlen_env_check(master, null_message)) {
                    printf("=====================================================\n");
                    zahlen_env_sum_up(master);
                    printf("=====================================================\n");
                }
            }; break;
            case 5: {
                if (zahlen_env_check(master, null_message)) {
                    printf("*****************************************************\n");
                    zahlen_env_destruct(master);
                    master = NULL;
                    printf("The linked list has been destructed!\n");
                    printf("*****************************************************\n");
                }
            }; break;
        }

        if (choice > 0) {
            pause();
        }
    } while (is_in_repl && !feof(stdin));

    return 0;
}

