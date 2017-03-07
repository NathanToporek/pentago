/* Nathanael Toporek
 * 3/7/15 TCSS 435
 * Programming Assignment #2
 * readme.txt
 */

Number of Nodes expanded is displayed at each point where the AI has built a game tree, since it builds its tree at each step.

The maximum number of plies I was able to look ahead for Pentago was only 3: AI->Human->AI.
Due to Pentago having a potential branching factor of 288, having the AI look ahead a fourth ply caused the game to take up between 4 and 8 GB of memory while running, and choosing a move takes about 40 seconds. I decided that this was infeasable and limited the AI to 3 plies of analysis, and I have yet to beat it.

Side Note: Due to me ignoring gamestates that have already been explored, the number of
           nodes expanded for each moe will not be equal, since the number of unique
           gamestates increases at each level until the end game states.
           I did this because of 288 possible opening moves, there will only be 36 different
           possible outcomes, so it makes sense (memory-usage wise) to ignore equal states.

On that note, this game will usually take up ~1GB of RAM if you're using Alpha Beta pruning, and about 2-4 GB of RAM if you're just doing plain minimax.

Complexity Stuff:

MiniMax w/ Alpha-Beta Pruning:

NUMBER OF NODES EXPANDED FOR MOVE #2 AT DEPTH 4: 271341
NUMBER OF NODES EXPANDED FOR MOVE #2 AT DEPTH 3: 2751

TIME COMPLEXITY: O(d^b), where d is the depth of the gametree built and b is the branching
                 factor of the game. Though Alpha-Beta Pruning does keep us from expanding 
                 a large sum of nodes, the worst case will always still be just plain ol'
                 MiniMax.

SPACE COMPLEXITY: See above.


Plain ol' MiniMax:

NUMBER OF NODES EXPANDED FOR MOVE #2 AT DEPTH 4: 5638947
NUMBER OF NODES EXPANDED FOR MOVE #1 AT DEPTH 3: 7036 

TIME COMPLEXITY: O(d^b), where d is the depth of our tree and b is the branching 
                 factor. 
                 
SPACE COMPLEXITY: See above.
