#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "gametree.h"

#define max(a, b)           ((a >= b) ? (a) : (b))
#define min(a, b)           ((a <= b) ? (a) : (b))

#define MEME_OVERLOAD       9001

#define MAX_DEPTH_MINIMAX   4
#define RESIZE_FACTOR	    36
#define MOVE_LEN            8

#define POSITIONS           9
#define BLOCKS              4
#define DIRECTIONS          2

#define RIGHT               1
#define LEFT                2

// Utility Values
#define POS_2_4_6_8         1
#define POS_1_3_7_9         3
#define POS_5               5
#define THREE_IN_A_ROW      20
#define TIE                 5000
#define WIN                 100000


void        __build_gt(gt_node* root, int depth);
gt_node*    __init_gt_node(void);
void        __destroy_gt_node(gt_node* node);
void        __calculate_utility(gt_node* node);
void        fail();
void        __translate(int block, int* x, int* y);
int         __check_diags(gt_node* node, int block);
int         __check_right(gt_node* node, int block, int row);
int         __check_down(gt_node* node, int block, int col);

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

void build_gt_minimax() {
	
	__build_gt(ROOT, 1);
}

void __build_gt(gt_node* root, int depth) {
	
	// If we've surpassed MAX_DEPTH, leave.
	if(depth >= MAX_DEPTH_MINIMAX) {
	    __calculate_utility(root);
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
                    // We can perform minimax as we generate this tree.
                    if(child != NULL) {
                        if(root->utility == 0) {
                            root->utility = child->utility; // If there is no utility. DO IT
                        } else if(root->state->currTurn == root->state->myTurn) { // MAX
                            root->utility = max(root->utility, child->utility);
                        } else if(root->state->currTurn != root->state->myTurn) { // MIN
                            root->utility = max(root->utility, child->utility);
                        }
                    }
                }
            }
        }
    }
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

void __calculate_utility(gt_node* node) {
    
    // We only calculate utility for leaf nodes.
    if(node->children->size != 0) {
        return;
    }
    int multiplier = 0;
    int utility = 0;
    GameState* state = node->state;
    // Add utility for controlling positions.
    for(int block = 1; block <= BLOCKS; block++) {
        for(int pos = 1; pos <= POSITIONS; pos++) {
            char piece = get_piece(state, block, pos);
            
            if(piece == state->myTurn && (piece == BLACK || piece == WHITE)) {
                multiplier = 1;
            } else if(piece != state->myTurn && (piece == BLACK || piece == WHITE)){
                multiplier = -1;
            } else {
                multiplier = 0;
            }
            if(pos == 5) {
                utility += POS_5 * multiplier;
            } else if(pos % 2) {
                utility += POS_1_3_7_9 * multiplier;
            } else {
                utility += POS_2_4_6_8 * multiplier;
            }
        }
    }
    int xmod = 0;
    int ymod = 0;
    // Check for 3 in a rows.
    for(int block = 1; block < BLOCKS; block++) {
        // Check for diagonal 3s
        utility += __check_diags(node, block);
        
        for(int pos = 0; pos < BLOCK_SIZE; pos++) {
            utility += __check_right(node, block, pos);
            utility += __check_down(node, block, pos);
        }
    }
    
    // Checking for wins/losses.
    if(state->blackWon && state->whiteWon) {
        utility -= TIE; // A tie is very bad.
    } else if(state->blackWon && state->myTurn == BLACK) {
        utility += WIN;
    } else if(state->whiteWon && state->myTurn == WHITE) {
        utility += WIN;
    } else if(state->blackWon && state->myTurn == WHITE) {
        utility -= WIN;
    } else if(state->whiteWon && state->myTurn == BLACK) {
        utility -= WIN;
    }
    node->utility = utility;
}

void fail() {
    printf("FATAL ERROR OCCURRED. EXITING.\n\n");
    exit(MEME_OVERLOAD);    
}

void __translate(int block, int* x, int* y) {
    
    switch(block) {
        case 2:
            *x += BLOCK_SIZE;
            break;
        case 3:
            *y += BLOCK_SIZE;
            break;
        case 4:
            *x += BLOCK_SIZE;
            *y += BLOCK_SIZE;
            break;
        default:
            break;
    }
}

int __check_diags(gt_node* node, int block) {
    char tl, tr;
    int x1 = 0, x2 = BLOCK_SIZE - 1, y = 0, utility = 0;
    __translate(block, &x1, &y);
    __translate(block, &x2, &y);
    // Get the chars we need to look at.
    tl = node->state->state[y][x1];
    tr = node->state->state[y][x2];

    int eq = TRUE;
    // Check top left, then check top right.
    if(tl == BLACK || tl == WHITE) {
        for(int inc = 1; inc < BLOCK_SIZE && eq; inc++) {
            eq &= (tl == node->state->state[y + inc][x1 + inc]);
        }
        
        if(eq && (tl == node->state->myTurn)) {
            utility += THREE_IN_A_ROW;
        } else if(eq && (tl != node->state->myTurn)) {
            utility -= THREE_IN_A_ROW;
        }
    }
    eq = TRUE;
    if(tr == BLACK || tr == WHITE) {
        for(int inc = 1; inc < BLOCK_SIZE; inc++) {
            eq &= (tr == node->state->state[y + inc][x2 - inc]);
        }
        if(eq && (tr == node->state->myTurn)) {
            utility += THREE_IN_A_ROW;
        } else if(eq && (tr != node->state->myTurn)) {
            utility -= THREE_IN_A_ROW;
        }
    }
    return utility;
}

int __check_right(gt_node* node, int block, int row) {

    char chr;
    int x = 0, y = row, utility = 0;
    __translate(block, &x, &y);
    chr = node->state->state[y][x];
    
    int eq = TRUE;
    if(chr == BLACK || chr == WHITE) {
        for(int i = 0; i < BLOCK_SIZE; i++) {
            eq &= (chr == node->state->state[y][x + i]);
        }    
        if(eq && chr == node->state->myTurn) {
            utility += THREE_IN_A_ROW;
        } else if (eq && (chr != node->state->myTurn)) {
            utility -= THREE_IN_A_ROW;
        }
    }
    return utility;
}

int __check_down(gt_node* node, int block, int col) {
    
    char chr;
    int x = col, y = 0, utility = 0;
    __translate(block, &x, &y);
    chr = node->state->state[y][x];
    
    int eq = TRUE;
    if(chr == BLACK || chr == WHITE) {
        for(int i = 0; i < BLOCK_SIZE; i++) {
            eq &= (chr == node->state->state[y + i][x]);
        }    
        if(eq && chr == node->state->myTurn) {
            utility += THREE_IN_A_ROW;
        } else if (eq && (chr != node->state->myTurn)) {
            utility -= THREE_IN_A_ROW;
        }
    }
    return utility;
}
