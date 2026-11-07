#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "tango-rules.h"

#define MOON_BLUE_THRESHOLD 150
#define SUN_RED_THRESHOLD   150
#define VERTICAL_OFFSET      86
#define HORIZONTAL_OFFSET    86
#define MOON_TILE             0
#define SUN_TILE              1
#define EMPTY_TILE           -1
#define EQUAL_SYMBOL          1
#define CROSS_SYMBOL          0

/**
 * Test tango puzzle
 * 0 0 0 0 0 0
 *            
 * 0 0 0 0 0 m
 *     =      
 * 0 s 0 m s 0
 *           x
 * 0 0 0 0 0 0
 *   =        
 * s 0 0x0 0 0
 *            
 * s 0 0 0 0 0
 */

int main(void)
{
	// Add horizontal symbols
	horizontalSymbolAdd(CROSS_SYMBOL, 8);

	// Add vertical symbols
	verticalSymbolAdd(EQUAL_SYMBOL, 10);
	verticalSymbolAdd(CROSS_SYMBOL, 12);
	verticalSymbolAdd(EQUAL_SYMBOL, 21);

	// Add pieces
	startingTileAdd(MOON_TILE,  0);
	startingTileAdd(SUN_TILE,   1);
	startingTileAdd(MOON_TILE,  2);
	startingTileAdd(MOON_TILE,  3);
	startingTileAdd(SUN_TILE,   4);
	startingTileAdd(SUN_TILE,   5); // Actual starter
	startingTileAdd(SUN_TILE,   6);
	startingTileAdd(MOON_TILE,  7);
	startingTileAdd(MOON_TILE,  8);
	startingTileAdd(SUN_TILE,   9);
	startingTileAdd(MOON_TILE, 10);
	startingTileAdd(SUN_TILE,  11); // Actual starter
	startingTileAdd(SUN_TILE,  12);
	startingTileAdd(MOON_TILE, 13);
	startingTileAdd(SUN_TILE,  14);
	startingTileAdd(SUN_TILE,  15);
	startingTileAdd(MOON_TILE, 16);
	startingTileAdd(MOON_TILE, 17);
	startingTileAdd(SUN_TILE,  19); // Actual starter
	startingTileAdd(SUN_TILE,  22); // Actual starter
	startingTileAdd(MOON_TILE, 20); // Actual starter
	startingTileAdd(MOON_TILE, 24); // Actual starter

	// Solver goes from left->right, top->bottom (MSB->LSB)

	assert(startingTileContains(17) == 1);
	assert(getTile(17) == 0);
	assert(getAboveTile(11) == 0);
	assert(getBelowTile(23) == 0);
	assert(getLeftTile(16)  == 0);
	assert(getRightTile(17) == 0);

	assert(getAboveSymbol(0) == 0);
	assert(getBelowSymbol(6) == 0);
	assert(getLeftSymbol(8)  == 1);
	assert(getRightSymbol(9) == 1);

	assert(isValid(0, 1) == 1);
	assert(isValid(1, 1) == 1);
	assert(isValid(0, 3) == 0);
	assert(isValid(1, 3) == 1);

	updateBitBoardList();

	for (int i=0; i < 12; i++)
	{
		printf("bboard: %llx\n",bbs[i]);
	}


	return 0;
}