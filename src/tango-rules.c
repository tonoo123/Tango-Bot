#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "tango-rules.h"

uint64_t LEFT_EDGE					= 0x0000000820820820ULL;
uint64_t RIGHT_EDGE					= 0x0000000041041041ULL;
uint64_t TOP_EDGE					= 0x0000000fc0000000ULL;
uint64_t BOTTOM_EDGE				= 0x000000000000003fULL;

uint64_t HORIZONTAL_DIVIDERS_CROSS	= 0x0000000000000000ULL;
uint64_t HORIZONTAL_DIVIDERS_EQUAL	= 0x0000000000000000ULL;
uint64_t VERTICAL_DIVIDERS_CROSS	= 0x0000000000000000ULL;
uint64_t VERTICAL_DIVIDERS_EQUAL	= 0x0000000000000000ULL;
uint64_t MOONS						= 0x0000000000000000ULL;
uint64_t SUNS						= 0x0000000000000000ULL;
uint64_t SUNS_MOONS_COMBINED		= 0x0000000000000000ULL;
uint64_t STARTING_SET				= 0x0000000000000000ULL;

uint64_t bbs[12] = {0,0,0,0,0,0,0,0,0,0,0,0};

static inline uint64_t isLeftCol(int b) { return LEFT_EDGE & (1ULL << b); };
static inline uint64_t isRightCol(int b) { return RIGHT_EDGE & (1ULL << b); };
static inline uint64_t isTopRow(int b) { return TOP_EDGE & (1ULL << b); };
static inline uint64_t isBottomRow(int b) { return BOTTOM_EDGE & (1ULL << b); };

/**
 * Checks for and returns the vertical dividor symbol to the left of a tile.
 * @param b Bit position.
 * @return -1 for no symbol,
 *		    0 for cross,
 *		    1 for equal.
 */
int getLeftSymbol(int b)
{
	int res = -1;
	if (isLeftCol(b))
		goto exit;
	uint64_t cross = (1ULL << b) & HORIZONTAL_DIVIDERS_CROSS;
	uint64_t equal = (1ULL << b) & HORIZONTAL_DIVIDERS_EQUAL;
	if (cross)
	{
		res = 0;
		goto exit;
	}
	else if (equal)
		res = 1;
exit:
	return res;
}

/**
 * Checks for and returns the vertical dividor symbol to the right of a tile.
 * @param b Bit position.
 * @return -1 for no symbol,
 *		    0 for cross,
 *		    1 for equal.
 */
int getRightSymbol(int b)
{
	int res = -1;
	if (isRightCol(b))
		goto exit;
	uint64_t cross = (1ULL << (b-1)) & HORIZONTAL_DIVIDERS_CROSS;
	uint64_t equal = (1ULL << (b-1)) & HORIZONTAL_DIVIDERS_EQUAL;
	if (cross)
	{
		res = 0;
		goto exit;
	}
	else if (equal)
		res = 1;
exit:
	return res;
}

/**
 * Checks for and returns the vertical dividor symbol above a tile.
 * @param b Bit position.
 * @return -1 for no symbol,
 * 		    0 for cross,
 * 		    1 for equal.
 */
int getAboveSymbol(int b)
{
	int res = -1;
	if (isTopRow(b))
		goto exit;
	uint64_t cross = (1ULL << b) & VERTICAL_DIVIDERS_CROSS;
	uint64_t equal = (1ULL << b) & VERTICAL_DIVIDERS_EQUAL;
	if (cross)
	{
		res = 0;
		goto exit;
	}
	else if (equal)
		res = 1;
exit:
	return res;
}

/**
 * Checks for and returns the horizontal dividor symbol below a tile.
 * @param b Bit position
 * @return -1 for no symbol,
 * 		    0 for cross,
 * 		    1 for equal.
 */
int getBelowSymbol(int b)
{
	int res = -1;
	if (isBottomRow(b))
		goto exit;
	uint64_t cross = (1ULL << (b-6)) & VERTICAL_DIVIDERS_CROSS;
	uint64_t equal = (1ULL << (b-6)) & VERTICAL_DIVIDERS_EQUAL;
	if (cross)
	{
		res = 0;
		goto exit;
	}
	else if (equal)
		res = 1;
exit:
	return res;
}

int getTile(int b)
{
	int res = -1;
	if ((MOONS >> b) & 1)
		res = 0;
	else if ((SUNS >> b) & 1)
		res = 1;
	return res;
}

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
	if (type)
		SUNS |= (1ULL << b);
	else
		MOONS |= (1ULL << b);
	SUNS_MOONS_COMBINED |= (1ULL << b);
	STARTING_SET 		|= (1ULL << b);
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
	if (STARTING_SET & (1ULL << b))
		res = 1;
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
		HORIZONTAL_DIVIDERS_EQUAL |= (1ULL << b);
	else
		HORIZONTAL_DIVIDERS_CROSS |= (1ULL << b);
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
		VERTICAL_DIVIDERS_EQUAL |= (1ULL << b);
	else
		VERTICAL_DIVIDERS_CROSS |= (1ULL << b);
}

// static int validRow(int type, int c, int b)
// {
// 	uint64_t like = SUNS;
// 	int res = 1;
// 	if (!type)
// 		like = MOONS;

// 	uint64_t nextTile		= (like >> (b+1)) & 1;
// 	uint64_t prevTile		= (like >> (b-1)) & 1;
// 	uint64_t tileAfterNext	= (like >> (b+2)) & 1;
// 	uint64_t tileBeforePrev	= (like >> (b-2)) & 1;

// 	// uint64_t range = (like << (58 - (30 - (b-c)))) >> 58;
// 	uint64_t range = (like >> (b-c)) & 0x3F; // 0x3F: all 1 row
// 	int sum = __builtin_popcountll(range);

// 	if (
// 			(c > 1 && tileBeforePrev && prevTile)
// 		||	(c > 0 && c < 5 && nextTile && prevTile)
// 		||	(c < 4 && tileAfterNext && nextTile)
// 		||	(sum == 3)
// 	)
// 	{
// 		res = 0;
// 	}

// 	return res;
// }

static int validRow(int type, int c, int b)
{
	uint64_t like = type ? SUNS : MOONS;
	int r = b - c;                          /* first bit of this row */

	/* Balance: at most 3 of each symbol per row */
	if (__builtin_popcountll((like >> r) & 0x3F) >= 3)
		return 0;

	/* No three consecutive: check every window that would contain b */
	if (c >= 2 && ((like >> (b - 2)) & 0x3) == 0x3)          /* b-2, b-1 */
		return 0;
	if (c >= 1 && c <= 4 && ((like >> (b - 1)) & 0x5) == 0x5) /* b-1, b+1 */
		return 0;
    if (c <= 3 && ((like >> b) & 0x6) == 0x6)                /* b+1, b+2 */
		return 0;

	return 1;
}

// static int validCol(int type, int r, int c, int b)
// {
// 	uint64_t like = SUNS;
// 	int res = 1;
// 	if (!type)
// 		like = MOONS;

// 	uint64_t aboveTile		= (like >> (b +6)) & 1;
// 	uint64_t belowTile		= (like >> (b -6)) & 1;
// 	uint64_t tileAboveAbove	= (like >> (b+12)) & 1;
// 	uint64_t tileBelowBelow	= (like >> (b-12)) & 1;
// 	int sum =
// 		  ((like >> c)      & 1)
// 		+ ((like >> (c+6))  & 1)
// 		+ ((like >> (c+12)) & 1)
// 		+ ((like >> (c+18)) & 1)
// 		+ ((like >> (c+24)) & 1)
// 		+ ((like >> (c+32)) & 1);
	
// 		if (
// 				(r > 1 && belowTile && tileBelowBelow)
// 			||	(r > 0 && r < 5 && belowTile && aboveTile)
// 			||	(r < 5 && aboveTile && tileAboveAbove)
// 			||	(sum == 3)
// 		)
// 		{
// 			res = 0;
// 		}

// 	return res;
// }

/* b == c + 6*r, so r and c are redundant with b; keep b for convenience. */
static int validCol(int type, int r, int c, int b)
{
    uint64_t like = type ? SUNS : MOONS;

    /* Balance: at most 3 of this symbol in the column (bits c, c+6, ..., c+30) */
    if (__builtin_popcountll((like >> c) & 0x41041041ULL) >= 3)
        return 0;

    /* No three consecutive vertically; check every window containing b */
    if (r >= 2 && ((like >> (b - 12)) & 0x41) == 0x41)        /* r-2, r-1 */
        return 0;
    if (r >= 1 && r <= 4 && ((like >> (b - 6)) & 0x1001) == 0x1001) /* r-1, r+1 */
        return 0;
    if (r <= 3 && ((like >> b) & 0x1040) == 0x1040)           /* r+1, r+2 */
        return 0;

    return 1;
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
	if (tile >= 0)
	{
		if ( (symbol == 0 && (tile == type)) || (symbol == 1 && (tile != type)) )
		{
			res = 0;
			goto exit;
		}
	}

	symbol = getBelowSymbol(b);
	tile = getBelowTile(b);
	if (tile >= 0)
	{
		if ( (symbol == 0 && (tile == type)) || (symbol == 1 && (tile != type)) )
		{
			res = 0;
			goto exit;
		}
	}

	symbol = getLeftSymbol(b);
	tile = getLeftTile(b);
	if (tile >= 0)
	{
		if ( (symbol == 0 && (tile == type)) || (symbol == 1 && (tile != type)) )
		{
			res = 0;
			goto exit;
		}
	}

	symbol = getRightSymbol(b);
	tile = getRightTile(b);
	if (tile >= 0)
	{
		if ( (symbol == 0 && (tile == type)) || (symbol == 1 && (tile != type)) )
			res = 0;
	}

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
// int isValid(int type, int b)
// {
// 	int res = 1;

// 	if (startingTileContains(b))
// 	{
// 		res = -1;
// 		goto exit;
// 	}

// 	int col = b%6;
// 	int row = b/6;
// 	if (!validRow(type, col, b) || !validCol(type, row, col, b) || !validSymbols(type, b))
// 		res = 0;

// exit:
// 	return res;
// }

int isValid(int type, int b)
{
    if (startingTileContains(b))
        return 0;                    /* see note below */

    int col = b % 6;
    int row = b / 6;                 /* row index 0..5 */

    return validRow(type, col, b)
        && validCol(type, row, col, b)
        && validSymbols(type, b);
}

/**
 * Places a piece on the board. Updates bitboards.
 * @param type 0 for moon, 1 for sun.
 * @param b Bit position.
 * @return 1 on success, 0 on failure.
 */
int placePiece(int type, int b)
{
	int res = 0;
	if (isValid(type, b) == 1)
	{
		if (type)
			SUNS |= (1ULL << b);
		else
			MOONS |= (1ULL << b);
		SUNS_MOONS_COMBINED |= (1ULL << b);
		res = 1;
	}
	
	return res;
}

/**
 * Removes a piece from the board.
 * @param b Bit position.
 * @return 1 on success, 0 on failure.
 */
int removePiece(int b)
{
	int res = 0;
	if (!startingTileContains(b))
	{
		uint64_t mask = 1ULL << b;
		if (SUNS & mask)
			SUNS &= ~mask;
		else
			MOONS &= ~mask;
		SUNS_MOONS_COMBINED &= ~mask;
		res = 1;
	}
	return res;
}

void updateBitBoardList()
{
	bbs[0]  = LEFT_EDGE;
	bbs[1]  = RIGHT_EDGE;
	bbs[2]  = TOP_EDGE;
	bbs[3]  = BOTTOM_EDGE;
	bbs[4]  = HORIZONTAL_DIVIDERS_CROSS;
	bbs[5]  = HORIZONTAL_DIVIDERS_EQUAL;
	bbs[6]  = VERTICAL_DIVIDERS_CROSS;
	bbs[7]  = VERTICAL_DIVIDERS_EQUAL;
	bbs[8]  = MOONS;
	bbs[9]  = SUNS;
	bbs[10] = SUNS_MOONS_COMBINED;
	bbs[11] = STARTING_SET;
}

void printVerticalSymbols(int msb)
{
	for (int i = msb; i >= (msb-5); i--)
	{
		int sym = getAboveSymbol(i);
		switch (sym)
		{
			case 0:
				printf("  ");
				break;
			case 1:
				printf("x ");
				break;
			case 2:
				printf("= ");
				break;
		}
	}
}

void printBoard()
{
	printf("\n");
	for (int i=35; i >= 0; i--)
	{
		// Print symbol to left (if valid)
		if (!isLeftCol(i))
		{
			int sym = getLeftSymbol(i);
			switch (sym)
			{
				case 0:
					printf(" ");
					break;
				case 1:
					printf("x");
					break;
				case 2:
					printf("=");
					break;
			}
		}
		// Print piece
		int piece = getTile(i);
		switch (piece)
		{
			case -1:
				printf("0");
				break;
			case 0:
				printf("m");
				break;
			case 1:
				printf("s");
				break;
		}
		// Print symbols below
		if (isRightCol(i) && i != 0)
		{
			printf("\n");
			printVerticalSymbols(i-1);
			printf("\n");
		}
	}
	printf("\n\n");
}
