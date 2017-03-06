#ifndef GAMETREE_H
#define GAMETREE_H

#include "linkedlist.h"
#include "boardstate.h"

typedef struct __tree_node{
    
    struct __tree_node*    	parent; // Parent node of the tree.
    llist_t*               	children; // Children of this node in the tree
    
    int         			utility; // Utility value for this gamestate. Only calculated on leaves.
    GameState*  			state; // State data for this node in the tree.
    char*       			move; // Move data
} gt_node;

/* Initializes this GameTree and returns a reference to root node of this tree. */
gt_node*        init_root(GameState* gs);
/* Recursively destroys this GameTree from the node passed in. */
void            destroy_gt(gt_node* root);
// Builds a gametree. Let's hope this works.
char*            build_gt_minimax();
// Builds a gametree using alpha-beta pruning.
char*           build_gt_abpruning();

int             contains_state(gt_node* node, GameState* gs);
/* Reorganizes this tree to have the gt_node with the specified GameState as the 
 * root node.
 */
gt_node*        reorg(GameState* gs);
/* Gets the best move from this tree, by calculating utility at each gamestate. */
char*           get_best_move(GameState* gs);

#endif
