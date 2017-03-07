/* Nathanael Toporek
 * 3/7/15 TCSS 435
 * Programming Assignment #2
 * driver.c
 *
 * The code that will actually handle two players playing the game.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "boardstate.h"
#include "gametree.h"

#define TURN_LEN            10
#define MEME_OVERLOAD       9001

const char* const AI = "AI";
const char* const HUMAN = "HUMAN";

// Pointer to the function that is going to be building our 
// gametree.
char*           (* game_type)(void);

GameState*      start_game(void);
char*           get_turn(void);
void            declare_winners(GameState* gs);

int main(int argc, char** argv) {
    
    GameState* gs = start_game();
    gt_node* root;
    if(gs == NULL) {
        puts("Ooops! Something went wront. Exiting now.");
        exit(MEME_OVERLOAD);
    }
    do {
        // Initialize a root node.
        root = init_root(gs);
        // Print Turn info.
        print_state(gs);
        printf("Current Turn: ");
        if(gs->currTurn == BLACK) {
            printf("BLACK (%s)\n\n", ((gs->currTurn == gs->myTurn) ? AI : HUMAN));
        } else if(gs->currTurn == WHITE) {
            printf("WHITE (%s)\n\n", ((gs->currTurn == gs->myTurn) ? AI : HUMAN));
        }
        
        char* turn = NULL;
        
        if(gs->currTurn != gs->myTurn) { // Human player.
            turn = get_turn();
            // Hopefully clear the screen. :D
	        puts("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
        } else if(gs->currTurn == gs->myTurn) { // AI player.
            // Build our AI's gametree.
            printf("BUILDING GAMETREE.\n\n");
            turn = malloc(MOVE_LEN * sizeof(char));
            memcpy(turn, game_type(), MOVE_LEN);
            // Hopefully clear the screen. :D
            puts("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
            printf("NODES EXPANDED: %d\n", EXPANDED);
            printf("THE AI WILL PLAY THE MOVE: %s\n", turn);
            printf("EXPECTED UTILITY: %d\n", root->utility);
        }
        
        if(!parse_move(gs, turn)) {
            puts("You probably tried to play in a taken spot. Silly.\n");
        } 
        destroy_gt(root);
        puts("");
        
        free(turn);
    } while(!(gs->blackWon || gs->whiteWon));
    print_state(gs);
    declare_winners(gs);
    
    destroy_GameState(gs);
    return 0;
}

GameState* start_game() {

    srand(time(NULL));
    int color = rand() % 2;
    
    // Have the user choose whether or not AB pruning or minimax
    // is used.
    int success = FALSE;
    do {
        printf("How would you like our AI to play today?:\n");
        printf("A) Alpha-Beta Pruning w/ MiniMax\n");
        printf("M) Just MiniMax\n");
        printf("Your choice (A/M): ");
        char choice;
        choice = getchar();
        if(choice == 'A') {
            game_type = build_gt_abpruning;
            success = TRUE;
        } else if (choice == 'M') {
            puts("Should be minimax");
            game_type = build_gt_minimax;
            success = TRUE;
        }else {
            puts("You dork, give me valid input!\n");
        }
    } while (!success);
    // Clear stdin.
    char c;
    while ((c = getchar()) != '\n' && c != EOF);
    
    GameState* gs = init_GameState();
    puts("\nGameState Initialized!\nChoosing the AI's turn now!\n");
    
    printf("The AI will play as ");
    // Select AI player based off of random number.
    if(color) {
        gs->myTurn = BLACK;
        printf("BLACK!\n\n");
    } else {
        gs->myTurn = WHITE;
        printf("WHITE!\n\n");
    }
    return gs;
}

char* get_turn() {
    
    char* turn = malloc(TURN_LEN * sizeof(char));
    
    puts("Please enter your move in the form of \"block/position block#Rotation\"");
    printf("Enter your move here: ");
    fgets(turn, TURN_LEN - 1, stdin);
    return turn;
}

void declare_winners(GameState* gs) {
    
    hasanyonewonyet(gs);
    if(gs->blackWon && gs->whiteWon) {
        puts("It's a tie!");
    } else if(gs->blackWon) {
        puts("Black won!");
    } else if(gs->whiteWon) {
        puts("White won!");
    }
}
