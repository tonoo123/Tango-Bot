#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "tango-rules.h"

/**
 * Test tango puzzle: (0 is empty, 1 is moon, 2 is sun)
 * 0 0=0x0 0 0
 *       =    
 * 0 0 0 0 0=0
 *            
 * 0 0 0 0 0 0
 *            
 * m m 0 0x0 0
 *            
 * 0 0 0=0 0 0
 *         = x
 * 0 m 0 m 0 0
 */

int main(void)
{
	// Add pieces
	startingTileAdd(0, 2);
	startingTileAdd(0, 4);
	startingTileAdd(0, 16);
	startingTileAdd(0, 17);

	assert(startingTileContains(17) == 1);
	assert(getTile(17) == 0);
	assert(getAboveTile(11) == 0);
	assert(getBelowTile(23) == 0);
	assert(getLeftTile(16)  == 0);
	assert(getRightTile(17) == 0);
	
	// Add horizontal symbols
	horizontalSymbolAdd(1, 8);
	horizontalSymbolAdd(0, 13);
	horizontalSymbolAdd(1, 24);
	horizontalSymbolAdd(0, 32);
	horizontalSymbolAdd(1, 33);
	// Add vertical symbols
	verticalSymbolAdd(0, 0);
	verticalSymbolAdd(1, 1);
	verticalSymbolAdd(1, 26);

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