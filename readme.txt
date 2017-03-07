Number of Nodes expanded is displayed at each point where the AI has built a game tree, since it builds its tree at each step.

The maximum number of plies I was able to look ahead for Pentago was only 3: AI->Human->AI.
Due to Pentago having a potential branching factor of 288, having the AI look ahead a fourth ply caused the game to take up between 4 and 8 GB of memory while running, and choosing a move takes about 40 seconds. I decided that this was infeasable and limited the AI to 3 plies of analysis, and I have yet to beat it.

On that note, this game will usually take up ~1GB of RAM if you're using Alpha Beta pruning, and about 2-3 GB of RAM if you're just doing plain minimax.

Complexity Stuff:

MiniMax w/ Alpha-Beta Pruning:

TIME COMPLEXITY: O(d^b), where d is the depth of the gametree built and b is the branching
                 factor of the game. Though Alpha-Beta Pruning does keep us from expanding 
                 a large sum of nodes, the worst case will always still be just plain ol'
                 MiniMax.

SPACE COMPLEXITY: See above.


Plain ol' MiniMax:

TIME COMPLEXITY: O(d^b), where d is the depth of our tree and b is the branching 
                 factor. 
                 
SPACE COMPLEXITY: See above.
