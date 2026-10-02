#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

uint64_t VERTICAL_LEFT_EDGE		= 0x0000000820820820ULL;
uint64_t VERTICAL_RIGHT_EDGE	= 0x0000000041041041ULL;
uint64_t HORIZONTAL_UP_EDGE		= 0x0000000fc0000000ULL;
uint64_t HORIZONTAL_DOWN_EDGE	= 0x000000000000003fULL;

uint64_t HORIZONTAL_DIVIDERS_CROSS;
uint64_t HORIZONTAL_DIVIDERS_EQUAL;
uint64_t VERTICAL_DIVIDERS_CROSS;
uint64_t VERTICAL_DIVIDERS_EQUAL;
uint64_t MOONS;
uint64_t SUNS;
uint64_t SUNS_MOONS_COMBINED;

static int startingTiles[36];
static int tileCount = 0;

static inline int isLeftCol(int b) { return VERTICAL_LEFT_EDGE & (1 << b); };
static inline int isRightCol(int b) { return VERTICAL_RIGHT_EDGE & (1 << b); };
static inline int isTopRow(int b) { return HORIZONTAL_UP_EDGE & (1 << b); };
static inline int isBottomRow(int b) { return HORIZONTAL_DOWN_EDGE & (1 << b); };

// TODO: Refactor to 1 exit point and exit early if top/bottom/right/leftmost
/**
 * Checks for and returns the vertical dividor symbol to the left of a tile.
 * @param b Bit position.
 * @return 0 for no symbol,
 *		   1 for cross,
 *		   2 for equal.
 */
int getLeftSymbol(int b)
{
	int res = 0;
	if (isLeftCol(b))
		goto exit;
	int cross = (1 << b) & HORIZONTAL_DIVIDERS_CROSS;
	int equal = (1 << b) & HORIZONTAL_DIVIDERS_EQUAL;
	if (cross)
	{
		res = 1;
		goto exit;
	}
	else if (equal)
		res = 2;
exit:
	return res;
}

/**
 * Checks for and returns the vertical dividor symbol to the right of a tile.
 * @param b Bit position.
 * @return 0 for no symbol,
 *		   1 for cross,
 *		   2 for equal.
 */
int getRightSymbol(int b)
{
	int res = 0;
	if (isRightCol(b))
		goto exit;
	int cross = (1 << (b-1)) & HORIZONTAL_DIVIDERS_CROSS;
	int equal = (1 << (b-1)) & HORIZONTAL_DIVIDERS_EQUAL;
	if (cross)
	{
		res = 1;
		goto exit;
	}
	else if (equal)
		res = 2;
exit:
	return res;
}

/**
 * Checks for and returns the horizontal dividor symbol above a tile.
 * @param b Bit position.
 * @return 0 for no symbol,
 * 		   1 for cross,
 * 		   2 for equal.
 */
int getAboveSymbol(int b)
{
	int res = 0;
	if (isTopRow(b))
		goto exit;
	int cross = (1 << b) & VERTICAL_DIVIDERS_CROSS;
	int equal = (1 << b) & VERTICAL_DIVIDERS_EQUAL;
	if (cross)
	{
		res = 1;
		goto exit;
	}
	else if (equal)
		res = 2;
exit:
	return res;
}

/**
 * Checks for and returns the horizontal dividor symbol below a tile.
 * @param b Bit position
 * @return 0 for no symbol,
 * 		   1 for cross,
 * 		   2 for equal.
 */
int getBelowSymbol(int b)
{
	int res = 0;
	if (isBottomRow(b))
		goto exit;
	int cross = (1 << (b-6)) & VERTICAL_DIVIDERS_CROSS;
	int equal = (1 << (b-6)) & VERTICAL_DIVIDERS_EQUAL;
	if (cross)
	{
		res = 1;
		goto exit;
	}
	else if (equal)
		res = 2;
exit:
	return res;
}
// END TODO
int getLeftTile(int b)
{
	int res = -1;
	if ((MOONS >> (b+1)) & 1)
		res = 0;
	else if ((SUNS >> (b+1)) & 1)
		res = 1;
	
	return res;
}

int getRightTile(int b)
{
	int res = -1;
	if ((MOONS >> (b-1)) & 1)
		res = 0;
	else if ((SUNS >> (b-1)) & 1)
		res = 1;
	
	return res;
}

int getAboveTile(int b)
{
	int res = -1;
	if ((MOONS >> (b+6)) & 1)
		res = 0;
	else if ((SUNS >> (b+6)) & 1)
		res = 1;
	
	return res;
}

int getBelowTile(int b)
{
	int res = -1;
	if ((MOONS >> (b-6)) & 1)
		res = 0;
	else if ((SUNS >> (b-6)) & 1)
		res = 1;
	
	return res;
}

/**
 * Adds a bit position to the starting set. Also updates bitboards.
 * @param type 0 for moon, 1 for sun.
 * @param b Bit position to add.
 * @return None
 */
void startingTileAdd(int type, int b)
{ 
	startingTiles[tileCount++] = b;
	if (type)
		SUNS |= (1 << b);
	else
		MOONS |= (1 << b);
	SUNS_MOONS_COMBINED |= (1 << b);
}

/**
 * Determines if a bit position is part of the starting set.
 * @param b Bit position.
 * @return 0 if b is not in the starting set,
 * 		   1 if it is.
 */
int startingTileContains(int b)
{
	int res = 0;
	for (int i=0; i < tileCount; i++)
	{
		if (startingTiles[i] == b)
		{
			res = 1;
			break;
		}
	}
	return res;
}

/**
 * Adds a symbol to the board. Updates bitboards. Bit position is the tile
 * to the right of the symbol.
 * @param type 0 for cross, 1 for equal.
 * @param b Bit position.
 * @return None
 */
void horizontalSymbolAdd(int type, int b)
{
	if (type)
		HORIZONTAL_DIVIDERS_EQUAL |= (1 << b);
	else
		HORIZONTAL_DIVIDERS_CROSS |= (1 << b);
}

/**
 * Adds a symbol to the board. Updates bitboards. Bit position is the tile
 * above the symbol.
 * @param type 0 for cross, 1 for equal.
 * @param b Bit position.
 * @return None
 */
void verticalSymbolAdd(int type, int b)
{
	if (type)
		VERTICAL_DIVIDERS_EQUAL |= (1 << b);
	else
		VERTICAL_DIVIDERS_CROSS |= (1 << b);
}

static int validRow(int type, int c, int b)
{
	uint64_t like = SUNS;
	int res = 1;
	if (!type)
		like = MOONS;

	int nextTile		= (like >> (b+1)) & 1;
	int prevTile		= (like >> (b-1)) & 1;
	int tileAfterNext	= (like >> (b+2)) & 1;
	int tileBeforePrev	= (like >> (b-2)) & 1;

	int range = (like << 58 - (30 - (b-c))) >> 58;
	int sum = __builtin_popcountll(range);

	if (
			(c > 1 && tileBeforePrev && prevTile)
		||	(c > 0 && c < 5 && nextTile && prevTile)
		||	(c < 4 && tileAfterNext && nextTile)
		||	(sum == 3)
	)
	{
		res = 0;
	}

	return res;
}

static int validCol(int type, int r, int c, int b)
{
	uint64_t like = SUNS;
	int res = 1;
	if (!type)
		like = MOONS;

	int aboveTile		= (like >> (b +6)) & 1;
	int belowTile		= (like >> (b -6)) & 1;
	int tileAboveAbove	= (like >> (b+12)) & 1;
	int tileBelowBelow	= (like >> (b-12)) & 1;
	int sum =
		  ((like >> c)      & 1)
		+ ((like >> (c+6))  & 1)
		+ ((like >> (c+12)) & 1)
		+ ((like >> (c+18)) & 1)
		+ ((like >> (c+24)) & 1)
		+ ((like >> (c+32)) & 1);
	
		if (
				(r > 1 && belowTile && tileBelowBelow)
			||	(r > 0 && r < 5 && belowTile && aboveTile)
			||	(r < 5 && aboveTile && tileAboveAbove)
			||	(sum == 3)
		)
		{
			res = 0;
		}

	return res;
}

/**
 * Checks if the piece satisfies the symbols around the tile.
 * @param type 0 for moon, 1 for sun.
 * @param b	   Bit position.
 * @return 0 if not satisfactory,
 * 		   1 if it is.
 */
static int validSymbols(int type, int b)
{
	int res = 1;
	
	int symbol = getAboveSymbol(b);
	int tile = getAboveTile(b);
	if ( (symbol == 1 && (tile == type)) || (symbol == 2 && (tile != type)) )
	{
		res = 0;
		goto exit;
	}

	symbol = getBelowSymbol(b);
	tile = getBelowTile(b);
	if ( (symbol == 1 && (tile == type)) || (symbol == 2 && (tile != type)) )
	{
		res = 0;
		goto exit;
	}

	symbol = getLeftSymbol(b);
	tile = getLeftTile(b);
	if ( (symbol == 1 && (tile == type)) || (symbol == 2 && (tile != type)) )
	{
		res = 0;
		goto exit;
	}

	symbol = getRightSymbol(b);
	tile = getRightTile(b);
	if ( (symbol == 1 && (tile == type)) || (symbol == 2 && (tile != type)) )
		res = 0;

exit:
	return res;
}

/**
 * Checks whether placing a sun or moon at the given bit position is valid.
 * @param type 0 for moon, 1 for sun.
 * @param b    Bit position.
 * @return 0 if the placement is invalid,
 *		  -1 if attempting to edit the starting set,
 *		   1 if the placement is valid.
 */
int isValid(int type, int b)
{
	int res = 1;

	if (startingSetContains(b))
	{
		res = -1;
		goto exit;
	}

	int col = b%6;
	int row = b-col;
	if (!validRow(type, col, b) || !validCol(type, row, col, b) || !validSymbols(type, b))
		res = 0;

exit:
	return res;
}

int placePiece(int type, int b)
{
	return 0;
}

int removePiece(int b)
{
	return 0;
}