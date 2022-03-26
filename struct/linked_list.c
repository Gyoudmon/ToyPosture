#include <stdio.h>

/*************************************************************************************************/
#define offsetof(type, field) ((size_t)&((type*)0)->field)
#define containerof(ptr, type, field) (type*)((char*)ptr - offsetof(type, field))

typedef struct linked_list_node {
    struct linked_list_node *next;
} linked_list_node_t;

typedef struct s {
    int id;
    unsigned short score;

    linked_list_node_t node;
} s_t;

/*************************************************************************************************/
int main(int argc, char* argv[]) {
    s_t storage = { 1, 96, { NULL }};
    linked_list_node_t* node = &storage.node;
    s_t *s = containerof(node, s_t, node);

    printf("%d\n", s->score);

    return 0;
}

