#include "../linked_list.h"
#include "../zahlen.h"

/*************************************************************************************************/
typedef struct pseudo_struct {
    char id;
    char name[20];
    zahlen_t* score;
    char unused[16];
} pseudo_struct_t;

pseudo_struct_t pseudo_instance;

__ffi__ long long pseudo_address = (long long)&pseudo_instance;

/*************************************************************************************************/
__ffi__ size_t test_offsetof(int field_idx, const char* desc) {
    size_t offset;

    switch (field_idx) {
        case 0: offset = offsetof(pseudo_struct_t, id); break;
        case 1: offset = offsetof(pseudo_struct_t, name); break;
        case 2: offset = offsetof(pseudo_struct_t, score); break;
        case 3: offset = offsetof(pseudo_struct_t, unused); break;
    }

    return offset;
}

__ffi__ long long test_containerof(int field_idx, const char* desc) {
    pseudo_struct_t* src = &pseudo_instance;
    pseudo_struct_t* ptr = NULL;

    switch (field_idx) {
        case 0: ptr = containerof(&src->id, pseudo_struct_t, id); break;
        case 1: ptr = containerof(&src->name, pseudo_struct_t, name); break;
        case 2: ptr = containerof(&src->score, pseudo_struct_t, score); break;
        case 3: ptr = containerof(&src->unused, pseudo_struct_t, unused); break;
    }

    return (long long)ptr;
}

