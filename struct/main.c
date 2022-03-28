#include <stdio.h>

#include "linked_list.h"

/*************************************************************************************************/
int main(int argc, char* argv[]) {
    s_t storage = { 1, 96, { NULL }};
    linked_list_node_t* node = &storage.node;
    s_t *s = containerof(node, s_t, node);

    printf("%d\n", s->score);

    return 0;
}

