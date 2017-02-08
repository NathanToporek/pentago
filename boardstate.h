#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WHITE           'w'
#define BLACK           'b'
#define BOARD_SIZE      6

#define TRUE            1
#define FALSE           0
// A little easier to read with regards to hasanyonewonyet()
#define YES             1
#define NO              0

typedef struct Game_State {

    char      	state[BOARD_SIZE][BOARD_SIZE];
    char        winners[2];
    char        currTurn;
    char        myTurn;
    int         isWon;
} GameState;

GameState*      init_GameState(void);
void            destroy_GameState(GameState* gs);
void			print_state(GameState* gs);
int             apply_move(GameState* gs, char* move);
int             apply_rotation(GameState* gs, int block, char dir);
int             hasanyonewonyet(GameState* gs);
