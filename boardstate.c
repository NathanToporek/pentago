#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "boardstate.h"

// Private functions. Use parse_move to apply a move.
void        apply_move(GameState* gs, int block, int pos);
void        apply_rotation(GameState* gs, int block, char dir);

// Private functions for hasanyonewonyet(). they need to be implemented.
int         checkdown(GameState* gs, int col);
int         checkup(GameState* gs, int col);
int         checkright(GameState* gs, int row);
int         checkleft(GameState* gs, int row);
// Private functions to check for a diagonal win. Used by hasanyonewonyet().
int         checktopleft(GameState* gs);
int         checktopright(GameState* gs);
int         checkbottomleft(GameState* gs);
int         checkbottomright(GameState* gs);
// Private functions used by the checking functions above.
int         isvalidcolor(char color);
int         hascolorwonyet(GameState* gs, char color);
void        setwinner(GameState* gs, char color);

GameState* init_GameState(void) {

    GameState* gs = malloc(1 * sizeof(GameState));
    // Fail if we can't malloc a gamestate.
    if(gs == NULL) {
        printf("Error initializing gamestate struct. Returning NULL.\n");
        return NULL;
    }
    gs->blackWon = FALSE;
    gs->whiteWon = FALSE;
    gs->currTurn = BLACK;
    gs->myTurn = '\0';
    return gs;
}

GameState* copy_GameState(GameState* gs) {
    
    // Fail if you can't even pasS ME A NON NULL GAMESTATE.
    if(gs == NULL) {
        return NULL;
    }
    GameState* cpy = malloc(1 * sizeof(GameState));
    // Fail if fatal allocation error occurs.
    if(cpy == NULL) {
        return NULL;
    }
    // Copying non-state based information.
    cpy->currTurn = gs->currTurn;
    cpy->blackWon = gs->blackWon;
    cpy->whiteWon = gs->whiteWon;
    cpy->myTurn = gs->myTurn;
    // Copying state information.
    for(int y = 0; y < BOARD_SIZE; y++) {
        for(int x = 0; x < BOARD_SIZE; x++) {
            cpy->state[y][x] = gs->state[y][x];
        }
    }
    return cpy;
}
// Copies gs into other, assuming that neither is null.
void copy_into(GameState* gs, GameState* other) {
    
    if(gs == NULL || other == NULL) {
        return;
    }
    
    // Copying non-state based information.
    other->currTurn = gs->currTurn;
    other->blackWon = gs->blackWon;
    other->whiteWon = gs->whiteWon;
    other->myTurn = gs->myTurn;
    
    for(int y = 0; y < BOARD_SIZE; y++) {
        for(int x = 0; x < BOARD_SIZE; x++) {
            other->state[y][x] = gs->state[y][x];
        }
    }
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
        if(i == (BLOCK_SIZE)) {
            puts("\t+-------+-------+");
        }
        printf("\t|");
        for(int j = 0; j < BOARD_SIZE; j++) {
            if(j == (BLOCK_SIZE)) {
                printf(" | ");
            }
            if(j < (BLOCK_SIZE)) {
                printf(" ");
            }
            if(gs->state[i][j] != BLACK && gs->state[i][j] != WHITE) {
                printf("*");
            } else {
                printf("%c", gs->state[i][j]);
            }
            if(j >= (BLOCK_SIZE)) {
                printf(" ");
            }
        }
        printf("|\n");
    }
    printf("\t+-------+-------+\n\n");
}

char get_piece(GameState* gs, int block, int pos) {
    // If you're stupid.
    if(gs == NULL || (block > BLOCKS || block < 1) || (pos > POSITIONS || pos < 1)) {
        return '\0';
    }
    int xmod = (pos - 1) % (BLOCK_SIZE);
    int ymod = (pos - 1) / (BLOCK_SIZE);
    

    switch(block) {
        case 2:
            xmod += (BLOCK_SIZE);
            break;
        case 3:
            ymod += (BLOCK_SIZE);
            break;
        case 4:
            xmod += (BLOCK_SIZE);
            ymod += (BLOCK_SIZE);
            break;
        default:
            break;
    }
    return gs->state[ymod][xmod];
}

// Parses the move and prepares it to be applied to the passed gamestate.
int parse_move(GameState* gs, char* move) {
    
    // Copy the move.    
    char* cpy = malloc(MOVE_LEN * sizeof(char));
    memcpy(cpy, move, MOVE_LEN);
    
    char *pos = strtok(cpy, " ");
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
    if(block > BLOCKS || block < 1 || slot > POSITIONS || slot < 1 || rotblock > BLOCKS || rotblock < 1) {
        printf("pls stp been redarted. gib valld mov pls.\n");
        return FALSE;
    }
    // Gets the direction the user wants to rotate the board.
    char direction = rot[1];
    if(direction != 'R' && direction != 'L') {
        printf("You are the reason I have to write this extra code. Stop existing, please.\n");
        return FALSE;
    }
    // Check if the move is valid.
    char piece = get_piece(gs, block, slot);
    // If the place on the board is taken, exit.
    if(piece == BLACK || piece == WHITE) {
        return FALSE;
    }
    // ACTUALLY APPLY THE MOVE.    
    apply_move(gs, block, slot);
    if(hasanyonewonyet(gs)) {
        return TRUE; // Exit TRUE, if someone's won. As the game/move has completed.
    }
    apply_rotation(gs, rotblock, direction);
    hasanyonewonyet(gs); // We don't need to conditionally do something here.
                         // Just set a winner, it's the engine's job to output that shit.
                         
    free(cpy);	
    return TRUE;
}

void apply_move(GameState* gs, int block, int pos) {
    
    int xmod = (pos - 1) % (BLOCK_SIZE);
    int ymod = (pos - 1) / (BLOCK_SIZE);

    switch(block) {
        case 2:
            xmod += (BLOCK_SIZE);
            break;
        case 3:
            ymod += (BLOCK_SIZE);
            break;
        case 4:
            xmod += (BLOCK_SIZE);
            ymod += (BLOCK_SIZE);
            break;
        default:
            break;
    }
    
    gs->state[ymod][xmod] = gs->currTurn;
    
    // Update whose turn it is.
    if(gs->currTurn == BLACK) {
        gs->currTurn = WHITE;
    } else if(gs->currTurn == WHITE) {
        gs->currTurn = BLACK;
    }
}

void apply_rotation(GameState* gs, int block, char dir) {
    
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
    } 
}

int hasanyonewonyet(GameState* gs) {
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
    int isAWin = TRUE;
    // If the color isn't BLACK or WHITE, exit FALSE.
    if(!isvalidcolor(color)) {
        return FALSE;
    }
    // If the color has already won, exit TRUE.
    if(hascolorwonyet(gs, color)) {
        return TRUE;
    }
    // Check for winner here.
    for(int y = 1; y < WIN_LEN && isAWin; y++) {
        isAWin = (gs->state[y][col] == color);
    }
    // Set a winner
    if(isAWin) {
        setwinner(gs, color);
    }
    return isAWin;
}
int checkup(GameState* gs, int col) {
    
    char color = gs->state[BOARD_SIZE - 1][col];
    int isAWin = TRUE;
    // If the color isn't BLACK or WHITE, exit FALSE.
    if(!isvalidcolor(color)) {
        return FALSE;
    }
    // If the color has already won, exit TRUE.
    if(hascolorwonyet(gs, color)) {
        return TRUE;
    }
    // Check for winner here.
    for(int y = BOARD_SIZE - 1; y >= (BOARD_SIZE - WIN_LEN) && isAWin; y--) {
        isAWin = (gs->state[y][col] == color);
    }
    // Set a winner
    if(isAWin) {
        setwinner(gs, color);
    }
    return isAWin;   
}
int checkright(GameState* gs, int row) {

    char color = gs->state[row][0];
    int isAWin = TRUE;
    // If the color isn't BLACK or WHITE, exit FALSE.
    if(!isvalidcolor(color)) {
        return FALSE;
    }
    // If the color has already won, exit TRUE.
    if(hascolorwonyet(gs, color)) {
        return TRUE;
    }
    // Check for winner here.
    for(int x = 0; x < WIN_LEN && isAWin; x++) {
        isAWin = (gs->state[row][x] == color);
    }
    // Set a winner
    if(isAWin) {
        setwinner(gs, color);
    }
    return isAWin;
}
int checkleft(GameState* gs, int row) {
    
    char color = gs->state[row][BOARD_SIZE - 1];
    int isAWin = TRUE;
    // If the color isn't BLACK or WHITE, exit FALSE.
    if(!isvalidcolor(color)) {
        return FALSE;
    }
    // If the color has already won, exit TRUE.
    if(hascolorwonyet(gs, color)) {
        return TRUE;
    }
    // Check for winner here.
    for(int x = BOARD_SIZE - 1; x >= (BOARD_SIZE - WIN_LEN) && isAWin; x--) {
        isAWin = (gs->state[row][x] == color);
    }
    // Set a winner
    if(isAWin) {
        setwinner(gs, color);
    }
    return isAWin;
}
// Code to check the four corners
// Checks for wins in the topleft corner, and the two diagonals 
int checktopleft(GameState* gs) {
    // We need inividual ints to check for each winner.
    int winners[3] = {0, 0, 0};
    // Check each diag here.
    for(int i = 0; i < 3; i++) {
        // Account for checking the short diags.
        int xmod = 0, ymod = 0;
        switch (i) {
            case 1:
                xmod = 1;
                break;
            case 2:
                ymod = 1;
                break;
            default:
                break;
        }
        // Get the right color
        char color = gs->state[ymod][xmod];
        int isvalid = isvalidcolor(color); // Is the color valid?
        int iswon = hascolorwonyet(gs, color); // Has the color already won?
        // If we've already won, we can ignore checking for a win.
        // Otherwise we check for a win.
        if(isvalid && iswon) {
            winners[i] = TRUE;  // We want to communicate that a winner has been
                                // declared. (Previously, that is.)
        } else if(isvalid && !iswon) {
            winners[i] = TRUE;
            // Actually check for a win.
            for(int j = 1; (j < WIN_LEN) && winners[i]; j++) {
                winners[i] = (gs->state[j + ymod][j + xmod] == color);
            }
            // If we've detected a win, set that winner. If control is false, we skip this.
            if(winners[i]) {
                setwinner(gs, color);
            }
        }
    }
    // If we detected a win here, communicate that.
    return winners[0] || winners[1] || winners [2]; 
}
int checktopright(GameState* gs) {
    // We need inividual ints to check for each winner.
    int winners[3] = {0, 0, 0};
    // Check each diag here.
    for(int i = 0; i < 3; i++) {
        // Account for checking the short diags.
        int xmod = BOARD_SIZE - 1, ymod = 0;
        switch (i) {
            case 1:
                xmod -= 1;
                break;
            case 2:
                ymod = 1;
                break;
            default:
                break;
        }
        // Get the right color
        char color = gs->state[ymod][xmod];
        int isvalid = isvalidcolor(color); // Is the color valid?
        int iswon = hascolorwonyet(gs, color); // Has the color already won?
        // If we've already won, we can ignore checking for a win.
        // Otherwise we check for a win.
        if(isvalid && iswon) {
            winners[i] = TRUE;  // We want to communicate that a winner has been
                                // declared. (Previously, that is.)
        } else if(isvalid && !iswon) {
            winners[i] = TRUE;
            // Actually check for a win.
            for(int j = 1; (j < WIN_LEN ) && winners[i]; j++) {
                winners[i] = (gs->state[ymod + j][xmod - j] == color);
            }
            // If we've detected a win, set that winner.
            if(winners[i]) {
                setwinner(gs, color);
            }
        }
    }
    // If we detected a win here, communicate that.
    return winners[0] || winners[1] || winners [2]; 
}
int checkbottomleft(GameState* gs) {

    int isawin = TRUE;
    // Account for the corner we're checking.
    int ymod = BOARD_SIZE - 1;
    char color = gs->state[ymod][0];
    // If the color isn't BLACK or WHITE, exit FALSE.
    if(!isvalidcolor(color)) {
        return FALSE;
    }
    // If the color has already won, exit TRUE.
    if(hascolorwonyet(gs, color)) {
        return TRUE;
    }
    // Check for a win here.
    for(int i = 1; i < WIN_LEN && isawin; i++) {
        isawin = (color == gs->state[ymod - i][i]);
    }
    if(isawin) {
        setwinner(gs, color);
    }
    return isawin;
}
int checkbottomright(GameState* gs) {
    
    int isawin = TRUE;
    // Account for the corner we're checking.
    int xmod = BOARD_SIZE - 1, ymod = BOARD_SIZE - 1;
    char color = gs->state[ymod][xmod];
    // If the color isn't BLACK or WHITE, exit FALSE.
    if(!isvalidcolor(color)) {
        return FALSE;
    }
    // If the color has already won, exit TRUE.
    if(hascolorwonyet(gs, color)) {
        return TRUE;
    }
    // Check for a win here.
    for(int i = 1; i < WIN_LEN && isawin; i++) {
        isawin = (color == gs->state[ymod - i][xmod - i]);
    }
    if(isawin) {
        setwinner(gs, color);
    }
    return isawin;
}

// Code to help the checkers above.
int isvalidcolor(char color) {
    return color == BLACK || color == WHITE;
}
int hascolorwonyet(GameState* gs, char color) {
    if(color == BLACK && gs->blackWon) {
        return TRUE;
    } else if(color == WHITE && gs->whiteWon) {
        return TRUE;
    }
    return FALSE;
}
void setwinner(GameState* gs, char color) {
    if(color == BLACK) {
        gs->blackWon = TRUE;
    } else if(color == WHITE) {
        gs->whiteWon = TRUE;
    }
}

// Code that determines if one GameState is equal to another.
int equals(GameState* gs, GameState* other) {
    
    int equal = TRUE;
    // If checking isn't necessary.
    if(gs == NULL || other == NULL) {
        equal &= FALSE;
    }
    equal &= (gs->blackWon == other->blackWon)
          && (gs->whiteWon == other->whiteWon)
          && (gs->currTurn == other->currTurn)
          && (gs->myTurn == other->myTurn);
    for(int y = 0; y < BOARD_SIZE && equal; y++) {
        for(int x = 0; x < BOARD_SIZE; x++) {
            equal &= (gs->state[y][x] == other->state[y][x]);
        }
    }
    return equal;
}
