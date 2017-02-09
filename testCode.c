#include "boardstate.h"

int main(int argc, char** argv) {
    
    printf("Initializing a gamestate object.\n\n");
    GameState* gs = init_GameState();
    
    print_state(gs);
    char move[] = "2/4 2L";
    printf("APPLYING MOVE %s\n\n", move);
    if(parse_move(gs, move))
        printf("RETURNED TRUE\n\n");
    print_state(gs);
    char mv[] = "2/7 2R";
    parse_move(gs, mv);
    print_state(gs);
    printf("Destroying a gamestate object.\n");
    destroy_GameState(gs);
}
