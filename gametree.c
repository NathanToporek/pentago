#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "gametree.h"

#define MEME_OVERLOAD   9001

#define MAX_DEPTH       5
#define RESIZE_FACTOR	36
#define MOVE_LEN        8

#define POSITIONS       9
#define BLOCKS          4
#define DIRECTIONS      2

#define RIGHT           1
#define LEFT            2

void        __build_gt(gt_node* root, int depth);
gt_node*    __init_gt_node(void);
void        __destroy_gt_node(gt_node* node);
void        fail();

gt_node*	ROOT;

gt_node* init_root(GameState* gs) {
    
    ROOT = __init_gt_node();
    // Fail if we can't even allocate ROOT or you can't pass a good state. Sad.
    if(ROOT == NULL || gs == NULL) {
        fail();
    }
    copy_into(gs, ROOT->state);
    
    return ROOT;
}

void destroy_gt(gt_node* root) {
    // Exit if you passed a null value.
    if(root == NULL) {
        return;
    }
    // Only recurse if we have children.
    if(root->children->size > 0) {

        llist_t* list = root->children;
        // Recurse into each child.
        while(list->head != NULL) {
            gt_node* child = (gt_node*) remove_node(list, list->head);
            destroy_gt(child);            
        }
    }
    __destroy_gt_node(root);
    return;
}

void build_gt() {
	
	__build_gt(ROOT, 1);
}

void __build_gt(gt_node* root, int depth) {
	
	// If we've surpassed MAX_DEPTH, leave.
	if(depth >= MAX_DEPTH) {
		return;
	}
	// Iterate over all moves
    for(int pblock = 1; pblock <= BLOCKS; pblock++) { // For each block
        for(int pos = 1; pos <= POSITIONS; pos++) { // And each position in that block
            for(int rblock = 1; rblock <= BLOCKS; rblock++) {//And all possible rotations
                for(int dir = 1; dir <= DIRECTIONS; dir++) {
                    // Make a babeh and the information associated with it.
                    gt_node* child = __init_gt_node();
                    // Make sure allocation is successful.
                    if(child == NULL) {
                        destroy_gt(ROOT);
                        fail();
                    }
                    // Copy the parent's state into the child's.
                    copy_into(root->state, child->state);
                    // Build move.
                    sprintf(child->move, "%d/%d %d", pblock, pos, rblock);
                    if(dir == RIGHT) {
                        child->move[5] = 'R';
                    } else if(dir == LEFT) {
                        child->move[5] = 'L';
                    }
                    child->move[6] = '\0';  
                    // We need to link the child to its parent and vice versa if
                    // the move got applied. Then we need to recurse.
                    if(parse_move(child->state, child->move) 
                    && !contains_state(root, child->state)) 
                    {
                        child->parent = root;
                        add_data(root->children, (void *) child);
                        // Now we can recurse into the child.
                        __build_gt(child, depth + 1);
                    } else {
                        __destroy_gt_node(child);
                    }
                }
            }
        }
    }
//    printf("I HAVE %d CHILDREN.\n\nHERE IS MY CHILD CLASS.\n\n", root->children->size);
//    print_state(root->state);
}

int contains_state(gt_node* node, GameState* gs) {
    
    int rval = FALSE;
    
    ll_node_t* curr = node->children->head;
    // Iterate over all the babehs.
    while(curr != NULL && !rval) {
    
        GameState* other = ((gt_node*) curr->data)->state;
        rval = equals(gs, other);
        
        curr = curr->next;
    }
    return rval;    
}

gt_node* __init_gt_node() {
    
    gt_node* node = malloc(1 * sizeof(gt_node));
    
    if(node == NULL) {
        return NULL;
    }
    node->parent = NULL;
    node->children = init_llist();
    node->utility = 0;
    node->state = init_GameState();
    node->move = malloc(MOVE_LEN * sizeof(char));
    
    return node;
}

// This assumes that the node has no children. !!!
void __destroy_gt_node(gt_node* node) {
    
    assert(node->children->size == 0);
    if(node != NULL) {
        destroy_GameState(node->state);
        free(node->children);
        free(node->move);
        free(node);
    }
}

void fail() {
    printf("FATAL ERROR OCCURRED. EXITING.\n\n");
    exit(MEME_OVERLOAD);    
}
