#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "boardstate.h"
#include "gametree.h"

#define TURN_LEN            10
#define MEME_OVERLOAD       9001

GameState*      start_game(void);
char*           get_turn(void);
void            declare_winners(GameState* gs);

int main(int argc, char** argv) {
    
    GameState* gs = start_game();
    gt_node* root;
    if(gs == NULL || root == NULL) {
        puts("Ooops! Something went wront. Exiting now.");
        exit(MEME_OVERLOAD);
    }
    do {
        // Build our AI's gametree.
        printf("BUILDING GAMETREE.\n\n");
        root = init_root(gs);
        build_gt_minimax();
        printf("ROOT'S UTILITY IS: %d\n\n", root->utility);
        // Print Turn info.
        print_state(gs);
        printf("Current Turn: ");
        if(gs->currTurn == BLACK) {
            printf("BLACK\n\n");
        } else if(gs->currTurn == WHITE) {
            printf("WHITE\n\n");
        }
        
        char* turn = get_turn();
        // Hopefully clear the screen. :D
        puts("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
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

    GameState* gs = init_GameState();
    puts("GameState Initialized!\nChoosing the AI's turn now!");

    srand(time(NULL));
    int color = rand() % 2;
    
    printf("The AI will play as ");
    // Select AI player based off of random number.
    if(color) {
        gs->myTurn = BLACK;
        printf("BLACK!\n");
    } else {
        gs->myTurn = WHITE;
        printf("WHITE!\n");
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
    
    if(gs->blackWon && gs->whiteWon) {
        puts("It's a tie!");
    } else if(gs->blackWon) {
        puts("Black won!");
    } else if(gs->whiteWon) {
        puts("White won!");
    }
}
