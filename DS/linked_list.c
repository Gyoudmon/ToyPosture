#include "linked_list.h"

#include <stdio.h>

/*************************************************************************************************/
#define reach_end(node, nil) ((node == nil) || (node == NULL))

/*************************************************************************************************/
static singly_list_node_t* singly_list_merge(singly_list_node_t* left, singly_list_node_t* right, singly_list_entry_datum_f entry_datum) {
    long long int ldatum = entry_datum(left);
    long long int rdatum = entry_datum(right);    
    singly_list_node_t* head = NULL;
    singly_list_node_t* tail = NULL;

    if (ldatum < rdatum) {
        head = left;
        left = left->next;
    } else {
        head = right;
        right = right->next;
    }

    tail = head;

    while ((left != NULL) && (right != NULL)) {
        ldatum = entry_datum(left);
        rdatum = entry_datum(right);
        
        if (ldatum < rdatum) {
            tail->next = left;
            left = left->next;
        } else {
            tail->next = right;
            right = right->next;
        }
        
        tail = tail->next;
    }

    if (left == NULL) {
        tail->next = right;
    } else {
        tail->next = left;
    }

    return head;
}

/*************************************************************************************************/
__lambda__ long long int singly_list_size(singly_list_node_t* head, singly_list_node_t* nil) {
    singly_list_node_t* self = head;
    long long int n = 0;

    while (!reach_end(self, nil)) {
        self = self->next;
        n++;
    }

    return n;
}

__lambda__ singly_list_node_t* singly_list_split_half(singly_list_node_t* head, singly_list_node_t* nil) {
    singly_list_node_t* pslow = head;
    singly_list_node_t* pfast = head;

    while (!reach_end(pfast, nil)) {
        pslow = pslow->next;
        pfast= pfast->next;
        
        if (!reach_end(pfast, nil)) {
            pfast = pfast->next;
        }
    }

    return pslow;
}

__lambda__ singly_list_node_t* singly_list_merge_sort(singly_list_node_t* head, singly_list_node_t* nil, singly_list_entry_datum_f entry_datum) {
    singly_list_node_t* newhead = NULL;

    if (head->next == nil) {
        head->next = NULL;
        newhead = head;
    } else {
        singly_list_node_t* mid_node = singly_list_split_half(head, nil);
        singly_list_node_t* left = singly_list_merge_sort(head, mid_node, entry_datum);
        singly_list_node_t* right = singly_list_merge_sort(mid_node, nil, entry_datum);
        
        newhead = singly_list_merge(left, right, entry_datum);
    }

    return newhead;
}

