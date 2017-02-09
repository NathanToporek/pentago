#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WHITE           'w'
#define BLACK           'b'
#define BOARD_SIZE      6
#define BLOCK_SIZE      BOARD_SIZE / 2
#define WIN_LEN         5

#define TRUE            1
#define FALSE           0

typedef struct Game_State {

    char      	state[BOARD_SIZE][BOARD_SIZE];
    char        currTurn;
    char        myTurn;
    int         blackWon;
    int         whiteWon;
} GameState;

GameState*      init_GameState(void);
void            destroy_GameState(GameState* gs);
void			print_state(GameState* gs);
int             parse_move(GameState* gs, char* move);
int             hasanyonewonyet(GameState* gs);
