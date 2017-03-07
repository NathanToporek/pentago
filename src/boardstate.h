/* Nathanael Toporek
 * 3/7/15 TCSS 435
 * Programming Assignment #2
 * boardstate.h
 *
 * Header file for boardstate.c, the code that runs the game.
 */

#ifndef BOARD_STATE_INCLUDE
#define BOARD_STATE_INCLUDE

#define WHITE           'w'
#define BLACK           'b'
#define BOARD_SIZE      6
#define BLOCK_SIZE      (BOARD_SIZE / 2)
#define WIN_LEN         5
#define MOVE_LEN        8

#define BLOCKS          4
#define POSITIONS       9

#define TRUE            1
#define FALSE           0
/**
 * Struct to hold information about a given gamestate. 
 * Containst the current boardstate, curr, and winners.
 */
typedef struct Game_State {

    char      	state[BOARD_SIZE][BOARD_SIZE];
    char        currTurn;
    char        myTurn;
    int         blackWon;
    int         whiteWon;
} GameState;
/**
 * Initializes a pointer to a gamestate and sets all fields to be either false or empty.
 */
GameState*      init_GameState(void);
/**
 * Performs a deep copy of the given gamestate.
 * 
 * Returns NULL if a failure occured, otherwise it returns a pointer to a copy gamestate.
 * 
 * DOES NOT FREE THE PASSED POINTER.
 */
GameState*      copy_GameState(GameState* gs);
/**
 * Copies the data from gs into other. 
 * Fails if either are null.
 */
void            copy_into(GameState* gs, GameState* other);
/**
 * Frees the passed gamestate. Does nothing otherwise.
 */
void            destroy_GameState(GameState* gs);
/**
 * Prints out gs->state.
 */
void			print_state(GameState* gs);
/**
 * Gets a piece from gs in block block and position pos.
 * 
 * Returns that peice if one is found, '\0' otherwise.
 */
char            get_piece(GameState* gs, int block, int pos);
/**
 * Tries to make sense of user input and apply a move to the gamestate.
 * 
 * Returns TRUE if success, FALSE otherwise.
 */
int             parse_move(GameState* gs, char* move);
/**
 * Determines if either player has won and sets the corresponding variables in gs.
 * 
 * Returnst TRUE if a winner was declared, FALSE otherwise.
 */
int             hasanyonewonyet(GameState* gs);
/**
 * Determines if two gamestates are equal by directly comparing each
 * struct variable in gs to that of the other, and iterating over all positions 
 * in each gamestate.
 *
 * Returns TRUE if both are equal, FALSE otherwise.
 */
int             equals(GameState* gs, GameState* other);

#endif
