#ifndef LINKEDLIST_H
#define LINKEDLIST_H

// LIST DESTRUCTION IS LEFT TO THE USER, AS I CAN'T NAIVELY FREE VOID POINTERS. 

/**
 * Struct to hold information about linked list.
 * Stores the head node and the number of children in this list.
 */
typedef struct __linked_list_t {
    
    struct __ll_node_t*         head;
    int                         size;
} llist_t;

/**
 * Contains information about a single linked list node.
 * Stores pointer to the previous and next node in the list as well as a pointer to the data this node is storing.
 */
typedef struct __ll_node_t {
    
    struct __ll_node_t*         prev;
    struct __ll_node_t*         next;
    
    void*                       data;
} ll_node_t;

// Initialize a Linked list.
llist_t*    init_llist(void);
// Add data to this Linked list.
// DATA MUST BE ALLOCATED ON THE HEAP.
int        add_data(llist_t* list, void* data);
// Remove a node from this linked list.
// Returns a reference to the data stored in node, for the user to free.
void*       remove_node(llist_t* list, ll_node_t* node);

#endif
