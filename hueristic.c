#include <stdio.h>
#include <stdlib.h>

#include "hueristic.h"

#define TRUE                1
#define FALSE               0

// Utility Values or Multipliers
#define TYPE_AB_MULT        2
#define TYPE_C_MULT         2
#define TIE                 5000
#define WIN                 100000

// Left/Down/LeftDown/LeftUp
const char TYPE_A_WIN[6] = {'*','*','*','*','*','\0'};
// Right/Up/RightDown/RightUp
const char TYPE_B_WIN[6] = {'\0','*','*','*','*','*'};
// The three block win.
const char TYPE_C_WIN[5] = {'*','*','*','*','*'};

// Checks for a clean type A win. Returns utility.
int         __typeA(GameState* gs, char* arr);
// Checks for a clean type B win. Returns utility.
int         __typeB(GameState* gs, char* arr);
// Checks for a clean type C win. Returns utility.
int         __typeC(GameState* gs, char* arr);
// Convert a collumn of a 2D array to a 1D array.
char* 		__to_array(int col, char state[][6]);
// Gets the six-length diags to 1D arrays.
char**      __diag_6(char state[][6]);
// Gets the five-length diags into 1D arrays.
char**      __diag_5(char state[][6]);

// Do all of the hueristics work in here.
int gen_hueristic(gt_node* node) {
    int utility = 0;
    GameState* gs = node->state;
    // Check for positble wins from each position.
    // Check rows.
    for(int y = 0; y < BOARD_SIZE; y++) {
        utility += __typeA(gs, gs->state[y]);
        utility += __typeB(gs, gs->state[y]);
    }
    // Check columns
    for(int x = 0; x < BOARD_SIZE; x++) {
        char* vert = __to_array(x, gs->state);
        utility += __typeA(gs, vert);
        utility += __typeB(gs, vert);
        
        free(vert);
    }
    // Get the 6-length diags.
    char **diag6 = __diag_6(gs->state);
    // Get their utility.
    utility += __typeA(gs, diag6[0]);
    utility += __typeA(gs, diag6[1]);
    utility += __typeB(gs, diag6[0]);
    utility += __typeB(gs, diag6[1]);
    // Free the 6-length diags.
    free(diag6[0]);
    free(diag6[1]);
    free(diag6);        
    
    // Get the 5-length diags.
    char **diag5 = __diag_5(gs->state);
    // Check for type C wins in the 3 panel wins.
	for(int i = 0; i < 4; i++) {
		utility += __typeC(gs, diag5[i]);
	}
    // Free the 5-length diags.
    for(int i = 0; i < 4; i++) {
        free(diag5[i]);
    }
    free(diag5);
    
    // Checking for wins/losses.
    if(gs->blackWon && gs->whiteWon) {
        utility -= TIE; // A tie is very bad.
    } else if(gs->blackWon && gs->myTurn == BLACK) {
        utility += WIN;
    } else if(gs->whiteWon && gs->myTurn == WHITE) {
        utility += WIN;
    } else if(gs->blackWon && gs->myTurn == WHITE) {
        utility -= WIN;
    } else if(gs->whiteWon && gs->myTurn == BLACK) {
        utility -= WIN;
    }
    return utility;
}

int __typeA(GameState* gs, char* arr) {
    
    int score = 0;
    int can_win = TRUE;
    char target = '\0'; // Updates once we find either BLACK or WHITE.
    
    for(int i = 0; i < BOARD_SIZE && can_win; i++) {
        
        if(TYPE_A_WIN[i] == '*' && (arr[i] == BLACK || arr[i] == WHITE)) {
            if(target == '\0') {
                target = arr[i]; // Declare our target color.
                score = TYPE_AB_MULT;
            } else if(arr[i] == target) {
                score *= score; // Score grows A LOT as we get more dots in a line.
            } else if (arr[i] != target) { // If we conflict with target, this is not a valid win.
                can_win = FALSE;
                score = 0;
            }
        }
    }
    // Accomodate for who we're scoring for.
    if(target != '\0') {
        if(target != gs->myTurn) { // Negate score only if this is MIN's score.
            score *= -1;
        }
    }   
    return score;
}

int __typeB(GameState* gs, char* arr) {
    
    int score = 0;
    int can_win = TRUE;
    char target = '\0'; // Updates once we find either BLACK or WHITE.
    
    for(int i = 0; i < BOARD_SIZE && can_win; i++) {
        
        if(TYPE_B_WIN[i] == '*' && (arr[i] == BLACK || arr[i] == WHITE)) {
            if(target == '\0') {
                target = arr[i]; // Declare our target color.
                score = TYPE_AB_MULT;
            } else if(arr[i] == target) {
                score *= score; // Score grows A LOT as we get more dots in a line.
            } else if (arr[i] != target) { // If we conflict with target, this is not a valid win.
                can_win = FALSE;
                score = 0;
            }
        }
    }
    // Accomodate for who we're scoring for.
    if(target != '\0') {
        if(target != gs->myTurn) { // Negate score only if this is MIN's score.
            score *= -1;
        }
    }   
    return score;
}

int __typeC(GameState* gs, char* arr) {
    
    int score = 0;
    int can_win = TRUE;
    char target = '\0'; // Updates once we find either BLACK or WHITE.
    
    for(int i = 0; (i < BOARD_SIZE - 1) && can_win; i++) {
        
        if(TYPE_A_WIN[i] == '*' && (arr[i] == BLACK || arr[i] == WHITE)) {
            if(target == '\0') {
                target = arr[i]; // Declare our target color.
                score = TYPE_C_MULT;
            } else if(arr[i] == target) {
                score *= score; // Score grows A LOT as we get more dots in a line.
            } else if (arr[i] != target) { // If we conflict with target, this is not a valid win.
                can_win = FALSE;
                score = 0;
            }
        }
    }
    // Accomodate for who we're scoring for.
    if(target != '\0') {
        if(target != gs->myTurn) { // Negate score only if this is MIN's score.
            score *= -1;
        }
    }   
    
    return score;
}

char* __to_array(int col, char state[][6]) {
    
    char* res = malloc((BOARD_SIZE + 1) * sizeof(char));
    for(int i = 0; i < BOARD_SIZE; i++) {
        res[i] = state[i][col];
    }
    return res;
}

char** __diag_6(char state[][6]) {

    char** res = malloc(2 * sizeof(char *));
    res[0] = malloc(BOARD_SIZE * sizeof(char)); // Rigt/down.
    res[1] = malloc(BOARD_SIZE * sizeof(char)); // Right/up.
    
    for(int y = 0; y < BOARD_SIZE; y++) {
        res[0][y] = state[y][y];
        res[1][y] = state[BOARD_SIZE - y - 1][y];
    }
    return res;
}

char** __diag_5(char state[][6]) {
    
    char** res = malloc(4 * sizeof(char *));
    
    for(int i = 0; i < 4; i++) {
        res[i] = malloc((BOARD_SIZE - 1) * sizeof(char)); 
    }
    
    for(int i = 0; i < (BOARD_SIZE - 1); i++) {
        res[0][i] = state[i][i + 1]; // Left, one right.
        res[1][i] = state[i + 1][i]; // Left, one down.
        res[2][i] = state[i][BOARD_SIZE - (i + 1)]; // Right, one left.
        res[3][i] = state[BOARD_SIZE - (i + 1)][i]; // Right, one down.
    }
    
    return res;
}

































