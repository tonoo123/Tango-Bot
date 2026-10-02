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

int startingSet[36];
int count = 0;

static inline int isLeftCol(int b) { return VERTICAL_LEFT_EDGE & (1 << b); };
static inline int isRightCol(int b) { return VERTICAL_RIGHT_EDGE & (1 << b); };
static inline int isTopRow(int b) { return HORIZONTAL_UP_EDGE & (1 << b); };
static inline int isBottomRow(int b) { return HORIZONTAL_DOWN_EDGE & (1 << b); };

int getVerticalLeft(int b)
{
	int cross = (1 << b) & VERTICAL_DIVIDERS_CROSS;
	int equal = (1 << b) & VERTICAL_DIVIDERS_EQUAL;
	if (cross)
		return 1;
	else if (equal)
		return 2;
	else
		return 0;
}

int getVerticalRight(int b)
{
	int cross = (1 << (b-1)) & VERTICAL_DIVIDERS_CROSS;
	int equal = (1 << (b-1)) & VERTICAL_DIVIDERS_EQUAL;
	if (cross)
		return 1;
	else if (equal)
		return 2;
	else
		return 0;
}

int getHorizontalUp(int b)
{
	int cross = (1 << b) & HORIZONTAL_DIVIDERS_CROSS;
	int equal = (1 << b) & HORIZONTAL_DIVIDERS_EQUAL;
	if (cross)
		return 1;
	else if (equal)
		return 2;
	else
		return 0;
}

int getHorizontalDown(int b)
{
	int cross = (1 << (b-6)) & HORIZONTAL_DIVIDERS_CROSS;
	int equal = (1 << (b-6)) & HORIZONTAL_DIVIDERS_EQUAL;
	if (cross)
		return 1;
	else if (equal)
		return 2;
	else
		return 0;
}

int startingSetInsert(int b) { startingSet[count++] = b; }

int startingSetContains(int b)
{
	int res = 0;
	for (int i=0; i < count; i++)
	{
		if (startingSet[i] == b)
		{
			res = 1;
			break;
		}
	}
	return res;
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
 * Checks if the placement of a sun or moon is valid in this tile. Returns false if 
 * this bit position is part of the starting set.
 * @param	int type: 0 for moon, 1 for sun.
 * @param	int b: bit position.
 * @return	int: 0 for invalid placement, -1 for starting set edit attempt, 1 for valid.
 */
int isValid(int type, int b)
{
	int res;

	if (startingSetContains(b))
	{
		printf("Cannot change starting set.\n");
		res = -1;
		goto exit;
	}

	int col = b%6;
	int row = b-col;

exit:
	return res;
}

int place(int type, int b)
{
	return 0;
}

int remove(int type, int b)
{
	return 0;
}