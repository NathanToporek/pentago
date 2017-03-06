#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "gametree.h"
#include "hueristic.h"

#define max(a, b)           ((a >= b) ? (a) : (b))
#define min(a, b)           ((a <= b) ? (a) : (b))

#define MEME_OVERLOAD       9001

#define MAX_DEPTH_MINIMAX   4
#define MAX_DEPTH_ABP       4
#define MOVE_LEN            8

#define POSITIONS           9
#define BLOCKS              4
#define DIRECTIONS          2

#define RIGHT               1
#define LEFT                2

#define INF                 100000000 // <-Not really inf, but w/e

void        __build_gt_mm(gt_node* root, int depth);
int         __build_gt_ab(gt_node* root, int depth, int alpha, int beta);
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

    // Initialize root's utility
    if(ROOT->state->currTurn == ROOT->state->myTurn) {
        ROOT->utility = -INF;
    } else {
        ROOT->utility = INF;
    }
    
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

char* build_gt_minimax() {
    // Builds the tree and does minimax at the same time.
	__build_gt_mm(ROOT, 1);
	
	// Finds the optimal move.
	int target = ROOT->utility;
	ll_node_t* curr = ROOT->children->head;
	while(curr != NULL) {
	    gt_node* node = ((gt_node *) curr->data);
	    // Exit if we've found our target
	    if(node->utility == target) {
	        return node->move;
	    }
	    curr = curr->next;
	}
	// If we get here we failed to find our target.
	return NULL;
}

char* build_gt_abpruning() {

    int alpha = -(INF);
    int beta = INF;
    // Builds the tree and does minimax at the same time.
	__build_gt_ab(ROOT, 1, alpha, beta);
	printf("Done with tree building. Children: %d", ROOT->children->size);
	// Finds the optimal move.
	int target = ROOT->utility;
	ll_node_t* curr = ROOT->children->head;
	while(curr != NULL) {
	    gt_node* node = ((gt_node *) curr->data);
	    // Exit if we've found our target
	    if(node->utility == target) {
	        return node->move;
	    }
	    curr = curr->next;
	}
	// If we get here we failed to find our target.
	return NULL;
}

void __build_gt_mm(gt_node* root, int depth) {
	
	// If we've surpassed MAX_DEPTH, leave.
	// OR WE'VE ALREADY WON. JESUS FUCK NO WONDER MY AI IS DUMB AS A BRICK.
	if(depth >= MAX_DEPTH_MINIMAX || root->state->blackWon || root->state->whiteWon) {
	    root->utility = gen_hueristic(root);
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
                        __build_gt_mm(child, depth + 1);
                    } else {
                        __destroy_gt_node(child);
                    }
                    // We can perform minimax as we generate this tree.
                    if(child != NULL) {
						if(root->utility == 0) {
                            root->utility = child->utility;						
                        } else if(root->state->currTurn == root->state->myTurn) { // MAX
                            root->utility = max(root->utility, child->utility);
                        } else { // MIN
                            root->utility = min(root->utility, child->utility);
                        }
                    }
                }
            }
        }
    }
}

int __build_gt_ab(gt_node* root, int depth, int alpha, int beta) {
    
    int myAlpha = alpha;
    int myBeta = beta;
    // If we've surpassed MAX_DEPTH, leave.
	// OR WE'VE ALREADY WON. JESUS FUCK NO WONDER MY AI IS DUMB AS A BRICK.
	if(depth >= MAX_DEPTH_ABP || root->state->blackWon || root->state->whiteWon) {
	    root->utility = gen_hueristic(root);
		return root->utility;
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
                    // This is also where we perform Alpha-Beta Pruning.
                    if((myAlpha < myBeta) 
                    && parse_move(child->state, child->move)
                    && !contains_state(root, child->state)) 
                    {
                        child->parent = root;
                        add_data(root->children, (void *) child);
	                    // Now we can recurse into the child.
                        int res = __build_gt_ab(child, depth + 1, myAlpha, myBeta);
                        
                        // Now we do our AB updates.
                        if(root->state->currTurn == root->state->myTurn) { // MAX
                            myAlpha = max(myAlpha, res);
                        } else { // MIN
                            myBeta = min(myBeta, res);
                        }
                    } else {
                        __destroy_gt_node(child);
                    }
                    // We can perform minimax as we generate this tree.
                    if(child != NULL) {
						if(root->utility == 0) { 
						    root->utility = child->utility;
						} else if(root->state->currTurn == root->state->myTurn) { // MAX
                            root->utility = max(root->utility, child->utility);
                        } else { // MIN
                            root->utility = min(root->utility, child->utility);
                        }
                    }
                }
            }
        }
    }
    return root->utility;
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

