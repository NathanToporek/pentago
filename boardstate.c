#include "boardstate.h"

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
        if(i == BOARD_SIZE / 2) {
            puts("\t+-------+-------+");
        }
        printf("\t|");
        
        for(int j = 0; j < BOARD_SIZE; j++) {
            if(j == BOARD_SIZE / 2) {
                printf(" | ");
            }
            if(j < BOARD_SIZE / 2) {
                printf(" ");
            }
            if(gs->state[i][j] != BLACK && gs->state[i][j] != WHITE) {
                printf("*");
            } else {
                printf("%c", gs->state[i][j]);
            }
            if(j >= BOARD_SIZE / 2) {
                printf(" ");
            }
        }
        
        printf("|\n");
    }
    
    printf("\t+-------+-------+\n\n");
}

int apply_move(GameState* gs, char* move) {
    
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
    int x = (slot - 1) % (BOARD_SIZE / 2);
    int y = (slot - 1) / (BOARD_SIZE / 2);
    // Accounting for each block.
    switch (block) {
        // Nothing needs to change here.
        case 1:
            gs->state[y][x] = gs->currTurn;
            break;
        case 2:
            // Move to top right block.
            x += BOARD_SIZE / 2;
            gs->state[y][x] = gs->currTurn;
            break;
        case 3:
            // Move to bottom left block.
            y += BOARD_SIZE / 2;
            gs->state[y][x] = gs->currTurn;
            break;
        case 4:
            // Move to bottom right block.
            x += BOARD_SIZE / 2;
            y += BOARD_SIZE / 2;
            gs->state[y][x] = gs->currTurn;
            break;
        default:
            // I don't know how we got here but we did.
            return FALSE;
    }
    
    //TODO Implement hasanyonewonyet()
    //hasanyonewonyet(gs);
    if(!apply_rotation(gs, rotblock, direction)) {
        return FALSE;
    }
    //TODO Implement hasanyonewonyet()
    //hasanyonewonyet(gs);
    return TRUE;
}

int apply_rotation(GameState* gs, int block, char dir) {
    
    // This is a private method that gets called in a method that sanitizes its input.
    // If you get my code to exit here, please tell me how.
    if(gs == NULL || block > 4 || block < 1 || (dir != 'R' && dir != 'L')) {
        printf("I shouldn't have to write this check, but people like you make this necessary. Thanks.\n");
       return FALSE;
    }
    char tmp[BOARD_SIZE / 2][BOARD_SIZE / 2];
    
    // Copy contents into tmp.
    for(int r = 0; r < BOARD_SIZE / 2; r++) {
        for(int c = 0; c < BOARD_SIZE / 2; c++) {
            // Handle which block we're operating in.
            switch(block) {
                case 1:
                    tmp[r][c] = gs->state[r][c];
                    break;
                case 2:
                    // Top right block.
                    tmp[r][c] = gs->state[r][c + BOARD_SIZE / 2];
                    break;
                case 3:
                    // Bottom left block.
                    tmp[r][c] = gs->state[r + BOARD_SIZE / 2][c];
                    break;
                case 4:
                    // Bottome right block.
                    tmp[r][c] = gs->state[r + BOARD_SIZE / 2][c + BOARD_SIZE / 2];
                    break;
            }
        }
    }
    if(dir == 'R') {
        // Right transpose into gs->state   
    }
    return TRUE;
}




































