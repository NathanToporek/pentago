#ifndef GAMETREE_H
#define GAMETREE_H

#include "boardstate.h"

typedef struct __tree_node{
    
    int         			maxchildren; // Upper bound on the number of children.

    int         			numchildren; // Number of kids.
    struct __tree_node*    	parent; // Parent node of the tree.
    struct __tree_node**   	children; // Children of this node in the tree
    
    int         			utility; // Utility value for this gamestate. Only calculated on leaves.
    GameState*  			state_data; // State data for this node in the tree.
    char*       			move; // Move data
} gt_node;

/* Initializes this GameTree and returns a reference to root node of this tree. */
gt_node*        init_ROOT(GameState* gs);
gt_node*        init_gt_node(void);
/* Recursively destroys this GameTree from the node passed in. */
void            destroy_gt(gt_node* root);
// Builds a gametree. Let's hope this works.
void            build_gt();
/* Reorganizes this tree to have the gt_node with the specified GameState as the 
 * root node.
 */
gt_node*        reorg(GameState* gs);
/* Gets the best move from this tree, by calculating utility at each gamestate. */
char*           get_best_move(GameState* gs);

#endif
