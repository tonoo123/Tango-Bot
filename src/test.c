#include <stdio.h>
#include <stdlib.h>
#include "tango-rules.h"

/**
 * Test tango puzzle: (0 is empty, 1 is moon, 2 is sun)
 * 0 0=0x0 0 0
 *       =    
 * 0 0 0 0 0=0
 *            
 * 0 0 0 0 0 0
 *            
 * 1 1 0 0x0 0
 *            
 * 0 0 0=0 0 0
 *         = x
 * 0 1 0 1 0 0
 */

int main(void)
{
	// Add pieces
	startingTileAdd(0, 2);
	startingTileAdd(0, 4);
	startingTileAdd(0, 16);
	startingTileAdd(0, 17);
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

	printBoard();

	return 0;
}