#include <stdio.h>
#include <stdlib.h>

#include "gametree.h"
#include "boardstate.h"
#include "linkedlist.h"

int main(int argc, char** argv) {
	
	GameState* gs = init_GameState();
	gt_node* root = init_root(gs);
	printf("SIZE OF GAMESTATE: %d\nSIZE OF GT_NODE: %d\n", sizeof(GameState), sizeof(gt_node));
	destroy_gt(root);
	destroy_GameState(gs);
}
