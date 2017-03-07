/* Nathanael Toporek
 * 3/7/15 TCSS 435
 * Programming Assignment #2
 * hueristic.h
 *
 * Header file for hueristic.c
 */

#ifndef HUERISTIC
#define HUERISTIC

#include "boardstate.h"
#include "gametree.h"
/**
 * Generates a hueristic for the given gt_node*.
 * Does so by looking at all possible wins for both players and adding 
 * 2^(number of matches in win type) to the hueristic if it's the AI's turn,
 * subtracting that number if its the human player's turn,
 * or adding 0 if that win is unatainable for either player.
 * 
 * Returns hueristic.
 */
int     gen_hueristic(gt_node* node);

#endif
