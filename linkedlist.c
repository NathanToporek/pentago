#include <stdio.h>
#include <stdlib.h>

#include "linkedlist.h"

#define SUCCESS         1
#define FAILURE         0

ll_node_t*          __init_node(void* data);

llist_t* init_llist(void) {
    
    // Allocate the new llist.
    llist_t* list = malloc(1 * sizeof(llist_t));
    // exit if allocation failed.
    if(list == NULL) {
        return NULL;
    }
    list->head = NULL;
    list->size = 0;
    
    return list;
}

int add_data(llist_t* list, void* data) {
    
    // Return if the list is NULL
    if(list == NULL) {
        return FAILURE;
    }
    
    ll_node_t* node = __init_node(data);
    // Exit if a fatal allocation error occured.
    if(node == NULL) {
        return FAILURE;
    }
    // Adds this node as the head of this list.
    // We don't need to keep track of a tail node now because the first node
    // added won't have a next reference, which is the definition of a tail.
    node->next = list->head;
    // Only update head->prev if the list wasn't empty.
    if (list->head != NULL) {
        list->head->prev = node;
    }
    list->head = node;
    list->size++;
    
    return SUCCESS;
}
/* It is your job to not be a fucking retard and pass a node from a different list.
 *
 * Returns a reference to the data in the deleted node, because I can't confidently
 * free a random void pointer. It may have pointers inside that need to be freed.
 * That's your job.
 */
void* remove_node(llist_t* list, ll_node_t* node) {
    
    // Exit if either is NULL
    if(list == NULL || node == NULL || list->size == 0) {
        return NULL;
    }
    
    ll_node_t* prev = node->prev;
    ll_node_t* next = node->next;
    
    if(next == NULL && prev == NULL) { // If the list only has one element.
        list->head = NULL;
    } else if (next == NULL) { // If we're freeing the tail.
        prev->next = NULL;
    } else if (prev == NULL) { // If we're freeing the head.
        next->prev = NULL;
        list->head = next;                
    } else { // Otherwise the node is in the middle of the list.
        prev->next = next;
        next->prev = prev;
    }
    void* ptr = node->data;
    list->size--;
    // Delete this node.
    free(node);
    return ptr;
}

ll_node_t* __init_node(void* data) {
    
    ll_node_t* node = malloc(1 * sizeof(ll_node_t));
    // Exit if a failure occured.
    if(node == NULL) {
        return NULL;
    }
    node->next = NULL;
    node->prev = NULL;
    node->data = data;
    
    return node;
}
