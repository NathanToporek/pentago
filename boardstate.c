#include "boardstate.h"

// Private functions. Use parse_move to apply a move.
int apply_move(GameState* gs, int block, int x, int y, char direction, int rotblock);
int apply_rotation(GameState* gs, int block, char dir);

// Private functions for hasanyonewonyet(). they need to be implemented.
int checkdown(GameState* gs, int col);
int checkup(GameState* gs, int col);
int checkright(GameState* gs, int row);
int checkleft(GameState* gs, int row);

GameState* init_GameState(void) {

    GameState* gs = malloc(1 * sizeof(GameState));
    // Fail if we can't malloc a gamestate.
    if(gs == NULL) {
        printf("Error initializing gamestate struct. Returning NULL.\n");
        return NULL;
    }
    gs->winners[0] = '\0';
    gs->winners[1] = '\0';
    gs->currTurn = BLACK;
    gs->isWon = FALSE;
    return gs;
}

void destroy_GameState(GameState *gs) {
    // We only need to actually do work if gs is not NULL.
    if(gs != NULL) {
        // Free the state. VIVA LA REVOLOTION.
        free(gs);
    }
}

void print_state(GameState* gs) {

    if(gs == NULL) {
        printf("rly dood, stop giving me null stuffs\n");
        return;
    }
    puts("CURRENT BOARDSTATE:\n");
    puts("\t+-------+-------+");
    // Lots of logic all up in heah, don't worry it should work.
    for(int i = 0; i < BOARD_SIZE; i++) {
        if(i == BLOCK_SIZE) {
            puts("\t+-------+-------+");
        }
        printf("\t|");
        for(int j = 0; j < BOARD_SIZE; j++) {
            if(j == BLOCK_SIZE) {
                printf(" | ");
            }
            if(j < BLOCK_SIZE) {
                printf(" ");
            }
            if(gs->state[i][j] != BLACK && gs->state[i][j] != WHITE) {
                printf("*");
            } else {
                printf("%c", gs->state[i][j]);
            }
            if(j >= BLOCK_SIZE) {
                printf(" ");
            }
        }
        printf("|\n");
    }
    printf("\t+-------+-------+\n\n");
}
// Parses the move and prepares it to be applied to the passed gamestate.
// TODO Maybe move this to another source file? Game Controller? idk, man
int parse_move(GameState* gs, char* move) {
    
    char *pos = strtok(move, " ");
    char *rot = strtok(NULL, "");
    
    if(pos == NULL || rot == NULL) {
        printf("u did a bad. pls gib valid mov. \n");
        return FALSE;
    }
    int block, slot, rotblock;
    // Getting the block, posiiton, rotation block numbers.
    block = pos[0] - '0';
    slot = pos[2] - '0';
    rotblock = rot[0] - '0';
    // If any of these are bad, tell the user to hecc off with their crap.
    if(block > 4 || block < 1 || slot > 9 || slot < 1 || rotblock > 4 || rotblock < 1) {
        printf("pls stp been redarted. gib valld mov pls.\n");
        return FALSE;
    }
    // Gets the direction the user wants to rotate the board.
    char direction = rot[1];
    if(direction != 'R' && direction != 'L') {
        printf("You are the reason I have to write this extra code. Stop existing, please.\n");
        return FALSE;
    }
    
    // ACTUALLY APPLY THE MOVE.
    int x = (slot - 1) % (BLOCK_SIZE);
    int y = (slot - 1) / (BLOCK_SIZE);
    
    return apply_move(gs, block, x, y, direction, rotblock);
}

int apply_move(GameState* gs, int block, int x, int y, char direction, int rotblock) {
    
    // Accounting for each block.
    switch (block) {
        case 2:
            // Move to top right block.
            x += BLOCK_SIZE;
            break;
        case 3:
            // Move to bottom left block.
            y += BLOCK_SIZE;
            break;
        case 4:
            // Move to bottom right block.
            x += BLOCK_SIZE;
            y += BLOCK_SIZE;
            break;
        default:
            break;
    }
    // If the place on the board is taken, exit.
    if(gs->state[y][x] == BLACK || gs->state[y][x] == WHITE) {
        printf("You did a bad. Someone already played here.\n");
        return FALSE;
    }
    gs->state[y][x] = gs->currTurn;
    
    //TODO Implement hasanyonewonyet()
    //hasanyonewonyet(gs);
    if(!apply_rotation(gs, rotblock, direction)) {
        return FALSE;
    }
    //TODO Implement hasanyonewonyet()
    //hasanyonewonyet(gs);
    
    // Update whose turn it is.
    if(gs->currTurn == BLACK) {
        gs->currTurn = WHITE;
    } else if(gs->currTurn == WHITE) {
        gs->currTurn = BLACK;
    }
    return TRUE;
}

int apply_rotation(GameState* gs, int block, char dir) {
    
    // This is a private method that gets called in a method that sanitizes its input.
    // If you get my code to exit here, please tell me how.
    if(gs == NULL || block > 4 || block < 1 || (dir != 'R' && dir != 'L')) {
        printf("I shouldn't have to write this check, but people like you make this necessary. Thanks.\n");
       return FALSE;
    }
    char tmp[BLOCK_SIZE][BLOCK_SIZE];
    
    // Get the x and y modifier for the block
    int xmod = 0, ymod = 0;
    switch(block) { 
        case 2:
            // Top right block.
            xmod = BLOCK_SIZE;
            break;
        case 3:
            // Bottom left block.
            ymod = BLOCK_SIZE;
            break;
        case 4:
            // Bottom right block.
            xmod = BLOCK_SIZE;
            ymod = BLOCK_SIZE;
            break;
        default:
            break;        
    }
    // Copy data into tmp
    for(int y = 0; y < BLOCK_SIZE; y++) {
        for(int x = 0; x < BLOCK_SIZE; x++) {
            tmp[y][x] = gs->state[y + ymod][x + xmod];
        }
    }
    // **Do the twist**
    if(dir == 'R') {
        // Right rotate tmp into gs->state   
        for(int y = 0; y < BLOCK_SIZE; y++) {
            for(int x = 0; x < BLOCK_SIZE; x++) {
                gs->state[y + ymod][x + xmod] = tmp[BLOCK_SIZE - x - 1][y];
            }
        }
    } else if(dir == 'L') {
        // Left rotate tmp into gs->state
        for(int y = 0; y < BLOCK_SIZE; y++) {
            for(int x = 0; x < BLOCK_SIZE; x++) {
                gs->state[y + ymod][x + xmod]= tmp[x][BLOCK_SIZE - y - 1];
            }
        }
    } else {
        printf("I'm beginning to think I check for errors too much.\n");
        return FALSE;
    }
    return TRUE;
}

int hasanyonewonyet(gs) {
    // A win has to be on an edge of the board. So we only need to check there.
    int status = FALSE;
    //Check for diagonal wins here
    status |= checktopleft(gs)
           || checktopright(gs)
           || checkbottomleft(gs)
           || checkbottomright(gs);
    // Check for vertical/lateral wins here.
    for(int num = 0; num < BOARD_SIZE; num++) {
        // Using bitwise OR to maintain status even if it becomes true.
        status |= checkdown(gs, num) 
               || checkup(gs, num) 
               || checkleft(gs, num) 
               || checkright(gs, num);
    }        
    return status;
}

int checkdown(GameState* gs, int col) {

    char color = gs->state[0][col];
    // If color isn't valid, leave.
    if(color != BLACK && color != WHITE) {
        return FALSE;
    }
    // If color has already won, leave.
    if((color == BLACK && gs->blackWon) || (color == WHITE && gs->whiteWon)) {
        return TRUE; // We're not counting the number of wins, so what I return
                     // here doesn't really matter.
    }
    int isAWin = TRUE;
    for(int y = 1; y < WIN_LEN && isAWin; y++) {
        isAWin = (gs->state[y][col] == color);
    }
    return isAWin;
}
int checkup(GameState* gs, int col) {
    
    char color = gs->state[BOARD_SIZE - 1][col];
    // If color isn't valid, leave.
    if(color != BLACK && color != WHITE) {
        return FALSE;
    }
    // If color has already won, leave.
    if((color == BLACK && gs->blackWon) || (color == WHITE && gs->whiteWon)) {
        return TRUE; // We're not counting the number of wins, so what I return
                     // here doesn't really matter.
    }
    int isAWin = TRUE;
    for(int y = BOARD_SIZE - 1; y >= (BOARD_SIZE - WIN_LEN) && isAWin; y--) {
        isAWin = (gs->state[y][col] == color);
    }
    return isAWin;   
}
int checkright(GameState* gs, int row) {

    char color = gs->state[row][0];
    // If color isn't valid, leave.
    if(color != BLACK && color != WHITE) {
        return FALSE;
    }
    // If color has already won, leave.
    if((color == BLACK && gs->blackWon) || (color == WHITE && gs->whiteWon)) {
        return TRUE; // We're not counting the number of wins, so what I return
                     // here doesn't really matter.
    }
    int isAWin = TRUE;
    for(int x = 0; x < WIN_LEN && isAWin; x++) {
        isAWin = (gs->state[row][x] == color);
    }
    return isAWin;
}
int checkleft(GameState* gs, int row) {
    
    char color = gs->state[row][BOARD_SIZE - 1];
    // If color isn't valid, leave.
    if(color != BLACK && color != WHITE) {
        return FALSE;
    }
    // If color has already won, leave.
    if((color == BLACK && gs->blackWon) || (color == WHITE && gs->whiteWon)) {
        return TRUE; // We're not counting the number of wins, so what I return
                     // here doesn't really matter.
    }
    int isAWin = TRUE;
    for(int x = BOARD_SIZE - 1; x >= (BOARD_SIZE - WIN_LEN) && isAWin; i--) {
        isAWin = (gs->state[row][x] == color);
    }
    return isAWin;
}

// Code to check the four corners
int checktopleft(GameState* gs) {}
int checktopright(GameState* gs) {}
int checkbottomleft(GameState* gs) {}
int checkbottomright(GameStat* gs) {}































