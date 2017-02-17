#include "boardstate.h"

typedef struct __tree_node{
    
    int         maxchildren; // Upper bound on the number of children.

    int         nchildren; // Number of kids.
    gt_node*    parent; // Parent node of the tree.
    gt_node**   children; // Children of this node in the tree
    
    int         utility; // Utility value for this gamestate. Only calculated on leaves.
    GameState*  state_data; // State data for this node in the tree.
    char*       move; // Move data
} gt_node;

/* Initializes this GameTree and returns a reference to root node of this tree. */
gt_node*        init_ROOT(void);
gt_node*        init_gt_node(void);
/* Recursively destroys this GameTree from the node passed in. */
void            destroy_gt(gt_node* root);
/* Reorganizes this tree to have the gt_node with the specified GameState as the 
 * root node.
 */
gt_node*        reorg(GameState* gs);
/* Gets the best move from this tree, by calculating utility at each gamestate. */
char*           get_best_move(GameState* gs);

