#ifndef LINKEDLIST_H
#define LINKEDLIST_H

// LIST DESTRUCTION IS LEFT TO THE USER, AS I CAN'T NAIVELY FREE VOID POINTERS. 

typedef struct __linked_list_t {
    
    struct __ll_node_t*         head;
    int                         size;
} llist_t;

// Doubly linked list node.
typedef struct __ll_node_t {
    
    struct __ll_node_t*         prev;
    struct __ll_node_t*         next;
    
    void*                       data;
} ll_node_t;

// Initialize a Linked list.
llist_t*    init_llist(void);
// Add data to this Linked list.
// DATA MUST BE ALLOCATED ON THE STACK.
int        add_data(llist_t* list, void* data);
// Remove a node from this linked list.
// Returns a reference to the data stored in node, for the user to free.
void*       remove_node(llist_t* list, ll_node_t* node);

#endif
