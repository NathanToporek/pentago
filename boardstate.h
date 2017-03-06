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

typedef struct Game_State {

    char      	state[BOARD_SIZE][BOARD_SIZE];
    char        currTurn;
    char        myTurn;
    int         blackWon;
    int         whiteWon;
} GameState;

GameState*      init_GameState(void);
GameState*      copy_GameState(GameState* gs);
void            copy_into(GameState* gs, GameState* other);
void            destroy_GameState(GameState* gs);
void			print_state(GameState* gs);
char            get_piece(GameState* gs, int block, int pos);
int             parse_move(GameState* gs, char* move);
int             hasanyonewonyet(GameState* gs);
int             equals(GameState* gs, GameState* other);

#endif
