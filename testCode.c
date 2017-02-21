#include <stdio.h>
#include <stdlib.h>

#include "gametree.h"
#include "boardstate.h"
#include "linkedlist.h"

int main(int argc, char** argv) {
	
	GameState* gs = init_GameState();
	gt_node* root = init_root(gs);
	printf("BUILDING GAMETREE.\n\n");
	build_gt();
	printf("DESTROYING GAMETREE.\n\n");
	destroy_gt(root);
	destroy_GameState(gs);
}
