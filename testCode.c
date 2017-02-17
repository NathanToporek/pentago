#include "boardstate.h"

int main(int argc, char** argv) {
    
    GameState* gs = init_GameState();
    GameState* gs2 = init_GameState();
    for(int i = 0; i < WIN_LEN; i++) {
        gs->state[i][i + 1] = BLACK;
        gs2->state[i][i + 1] = BLACK;
    }    
    print_state(gs);
    print_state(gs2);
//    hasanyonewonyet(gs);   
    
    printf("THE TWO STATES ARE %d\n\n", equals(gs, gs2));
    
    destroy_GameState(gs);
}
