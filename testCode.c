#include "boardstate.h"

int main(int argc, char** argv) {
    
    printf("Initializing a gamestate object.\n\n");
    GameState* gs = init_GameState();
    
    print_state(gs);
    char move[] = "2/4 1R";
    printf("APPLYING MOVE %s\n\n", move);
    apply_move(gs, move);
    print_state(gs);
    printf("Destroying a gamestate object.");
    destroy_GameState(gs);
}
