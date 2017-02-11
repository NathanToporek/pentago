#include "boardstate.h"

int main(int argc, char** argv) {
    
    printf("Initializing a gamestate object.\n\n");
    GameState* gs = init_GameState();
    
    print_state(gs);
    char move1[] = "1/1 2R";
    char move2[] = "1/4 4R";
    char move3[] = "1/2 2R";
    char move4[] = "1/5 2L";
    char move5[] = "1/3 2L";
    char move6[] = "1/6 2L";
    char move7[] = "2/1 3L";
    char move8[] = "2/4 3L";
    char move9[] = "2/2 1L";
    int success;
    printf("APPLYING MOVE: %s\n", move1);
    parse_move(gs, move1);
    print_state(gs);
    
    printf("APPLYING MOVE: %s\n", move2);
    parse_move(gs, move2);
    print_state(gs);
    
    printf("APPLYING MOVE: %s\n", move3);
    parse_move(gs, move3);
    print_state(gs);
    
    printf("APPLYING MOVE: %s\n", move4);
    parse_move(gs, move4);
    print_state(gs);
    
    printf("APPLYING MOVE: %s\n", move5);
    parse_move(gs, move5);
    print_state(gs);
    
    printf("APPLYING MOVE: %s\n", move6);
    parse_move(gs, move6);
    print_state(gs);
    
    printf("APPLYING MOVE: %s\n", move7);
    parse_move(gs, move7);
    print_state(gs);
    
    printf("APPLYING MOVE: %s\n", move8);
    parse_move(gs, move8);
    print_state(gs);
    
    printf("APPLYING MOVE: %s\n", move9);
    parse_move(gs, move9);
    print_state(gs);
    
}
