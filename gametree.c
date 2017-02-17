#include "gametree.h"

#define MEME_OVERLOAD   9001

#define MAX_DEPTH       3
#define RESIZE_FACTOR	5
#define MOVE_LEN        8

#define NUM_POS         9
#define BLOCKS          4
#define DIRECTIONS      2

#define RIGHT           1
#define LEFT            2

gt_node*	ROOT;

gt_node* init_ROOT(GameState* gs) {
	
    ROOT = malloc(1 * sizeof(gt_node));

    ROOT->parent = NULL; // Root node has no parent
	ROOT->children = NULL; // And is currently a leaf node.
	ROOT->nchildren = 0;
    ROOT->state_data = copy_GameState(gs);
    ROOT->utility = 0;
    ROOT->move = NULL;
    
    return ROOT;
}

gt_node* init_gt_node() {

    gt_node* node = malloc(1 * sizeof(gt_node));
    // If allocation fails, return NULL
    if(node == NULL) {
        return NULL;
    }
    node->nchildren = 0;
    node->utility = 0;
    
    return node;
}
// Recursively destroys this gametree.
void destroy_gt(gt_node* root) {
    // Base case: This root node has no children
    if(root->nchildren == 0) {
        destroy_GameState(root->state_data);
        if(root->move != NULL) {
            free(root->move);
        }
        free(root->children);
        free(root);
    } else { // For each child, recursively free it.
        for(; root->nchildren > 0; root->children--) {
            destroy_gt(root->children[nchildren - 1]);
        }
        free(root->children);
        // Then free this node.
        free(root);
    }
}

/* Build a tree with depth MAX_DEPTH using the passed GameState to permute.
 * THIS ASSUMES AN EMPTY TREE. WILL IMPLEMENT EXPAND_GT LATER TO EXPAND DEPTH.
 *
 * This also assumes that ROOT has been initialized with a gamestate.
 */
void build_gt() {
    
    __build_gt(ROOT, 0);
}

void __build_gt(gt_node* root, int depth) {
    // Base Case: if we're deep enough in the tree, leave.    
    if(depth >= MAX_DEPTH) {
        return;
    }
    // For every position, and every block rotation, try applying moves.
    // Branching factor: 288. Kill me now.
    for(int pblock = 1; plock <= BLOCKS; pblock++) { // Position block
        for(int pos = 1; pos <= NUM_POS; pos++) { // Position
            for(int rblock = 1; rblock <= BLOCKS; rblock++) { // Rotation block
                for(int dir = 1; dir <= DIRECTIONS; dir++) { // Rotation direction
                    // Allocate a move string.
                    char* move = malloc(MOVE_LEN * sizeof(char));
                    if(dir == LEFT) { // Handle if we're rotating left
                        sprintf(move, "%d/%d %dL\0", pblock, pos, rblock);
                    } else if(dir RIGHT) { // Handle if we're rotating right.
                        sprintf(move, "%d/%d %dR\0", pblock, pos, rblock);
                    }
                    // Copy the gamestate.
                    GameState* state = copy_GameState(root->state_data);
                    // Try to apply the move.
                    // If it succeeds:
                    // Add a child to the node, add the move and state to that node.
                    if(parse_move(state,  move)) {
                        gt_node* child = __add_child(root);
                        // Set up the child with its state information 
                        child->state_data = state;
                        child->move = move;
                        // Expand the child node.
                        __build_gt(child, depth + 1);
                    } 
                    // If it fails. Free move and destroy our copy of the gamestate.
                    else { 
                        free(move);
                        destroy_GameState(state);
                    }
                }
            }
        }
    }
    // Check for equal child nodes, and prune them, then reallocate the children array.
    int numequal = 0;
    for(int i = 0; i < root->numchildren; i++) {
        gt_node* a = root->children[i];
        if(a != NULL) { // Only check if the node isn't NULL
            for(int j = 0; j < root->numchildren; j++) {
                gt_node* b = root->children[j];
                if((i != j) && (b != NULL) && (equals(a->state_data, b->state_data))) { // If we're looking at two unique and equal states.
                    numequal++;
                    destroy_gt(b);
                }
            }        
        }
    }
}

/* Adds a new child node to the passed parent, then returns a reference to the child.
 * 
 * If necessary this will resize the parent's array of children.
 * The child will be initialized with blank values. See init_gt_node()
 *
 * If allocation of any kind fails, I'm going to assume that we've ran out of RAM
 * and this will end the program. This data structure is going to be huge.
 */
gt_node* __add_child(gt_node* parent) {
    
    // If parent doesn't have children, MAKE SOME BABEHS
    if(parent->children == NULL) {
        // Try to make room for some children.
        parent->children = malloc(RESIZE_FACTOR * sizeof(gt_node*));
        // If that fails, we're out of RAM. Exit.
        if(parent->children == NULL) {
            printf("FATAL ALLOCATION ERROR. EXITING NOW.\n");
            destroy_gt(ROOT); // Free the entire tree, minimizing our memory leakage.
            exit(MEME_OVERLOAD);
        }
        parent->maxchildren = RESIZE_FACTOR;
    }
    // Else if we need more children. Make them. 
    else if(parent->numchildren == parent->maxchildren) {
        parent->children = realloc(parent->children, parent->maxchildren + RESIZE_FACTOR);
        // If reallocation fails, we're out of RAM, exit.
        if(parent->children == NULL) {
            printf("FATAL ALLOCATION ERROR. EXITING NOW.\n");
            destroy_gt(ROOT);
            exit(MEME_OVERLOAD);
        }
        parent->maxchildren += RESIZE_FACTOR;
    }
    // Now that making babehs is out of the way, we make the child.
    gt_node* child = init_gt_node();
    // Link the child with the parent and vice versa.
    child->parent = parent;
    parent->children[numchildren++] = child;
    
    return child;
}
/** 
 * Reorganizes this tree around the most recent move.
 * This may take a bit to delete all of the univisited states.
 *
 * Returns the root node of the tree.
 *
 * THIS METHOD ASSUMES YOU REORGANIZE THE TREE AFTER EVERY MOVE IS MADE.
 * IT ONLY SEARCHES ONE LEVEL BELOW IN THE TREE FOR THE GAMESTATE.
 */
gt_node* reorg(GameState* gs) {
    
}

char* get_best_move() {

}
