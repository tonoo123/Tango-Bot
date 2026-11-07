#include "tango-rules.h"
#include "screenshot.h"
#include <stdio.h>
#include <stdint.h>
#include <windows.h>

// Purely to get rid of VScode undefined error, code will run perfectly fine without it
#ifndef DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2
    DECLARE_HANDLE(DPI_AWARENESS_CONTEXT);
    #define DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2 ((DPI_AWARENESS_CONTEXT)-4)
#endif

#define MOON_BLUE_THRESHOLD 150
#define SUN_RED_THRESHOLD   150
#define VERTICAL_OFFSET      86
#define HORIZONTAL_OFFSET    86
#define MOON_TILE             0
#define SUN_TILE              1
#define EMPTY_TILE           -1
#define EQUAL_SYMBOL          1
#define CROSS_SYMBOL          0

int width;
int height;

int topLeft[2] = {743, 390};
int botRght[2] = {1176, 823};

void printBoards()
{
	updateBitBoardList();
	printf("bbs[0]  = LEFT_EDGE;\nbbs[1]  = RIGHT_EDGE;\nbbs[2]  = TOP_EDGE;\nbbs[3]  = BOTTOM_EDGE;\nbbs[4]  = HORIZONTAL_DIVIDERS_CROSS;\n");
	printf("bbs[5]  = HORIZONTAL_DIVIDERS_EQUAL;\nbbs[6]  = VERTICAL_DIVIDERS_CROSS;\nbbs[7]  = VERTICAL_DIVIDERS_EQUAL;\nbbs[8]  = MOONS;\n");
	printf("bbs[9]  = SUNS;\nbbs[10] = SUNS_MOONS_COMBINED;\nbbs[11] = STARTING_SET;\n");
	for (int i=0; i < 12; i++)
	{
		printf("bboard: %llx\n",bbs[i]);
	}

}

int getPixelIndex(int x, int y)
{
	// Pixels have RGBA, so multiply by 4
	return (y*width + x) * 4;
}

void solve()
{
	for (int i=0; i < 6; i++)
	{
		for (int j=0; j < 6; j++)
		{
			int bitPos = (5-i)*6 + (5-j);
			if (getTile(bitPos) == EMPTY_TILE)
			{
				if (isValid(MOON_TILE, bitPos))
				{
					placePiece(MOON_TILE, bitPos);
					solve();
					removePiece(bitPos);
				}
				if (isValid(SUN_TILE, bitPos))
				{
					placePiece(SUN_TILE, bitPos);
					solve();
					removePiece(bitPos);
				}
				return;
			}
		}
	}
	printBoards();
}

int main(void)
{
	// // Make process DPI aware (takes into account screen resolution and scale)
	// SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

	// // Setup and take screenshot, load pixel data into buffer
	// width = GetSystemMetrics(SM_CXSCREEN);
	// height = GetSystemMetrics(SM_CYSCREEN);
	// initHandles_Bitmap(width, height);
	// unsigned char *buf = capture(0, 0);

	// // Read in tiles from Tango, starting at top left (MSB)
	// for (int i=0; i < 6; i++)
	// {
	// 	for (int j=0; j < 6; j++)
	// 	{
	// 		int x = topLeft[0] + (HORIZONTAL_OFFSET*j);
	// 		int y = topLeft[1] + (VERTICAL_OFFSET*i);
	// 		int ind = getPixelIndex(x, y);
	// 		unsigned char r = buf[ind+2];
	// 		unsigned char b = buf[ind];
	// 		// Sun
	// 		if (r > SUN_RED_THRESHOLD && b < MOON_BLUE_THRESHOLD)
	// 		{
	// 			int bitPos = (5-i)*6 + (5-j);
	// 			startingTileAdd(SUN_TILE, bitPos);
	// 		}
	// 		// Moon
	// 		else if (b > MOON_BLUE_THRESHOLD && r < SUN_RED_THRESHOLD)
	// 		{
	// 			int bitPos = (5-i)*6 + (5-j);
	// 			startingTileAdd(MOON_TILE, bitPos);
	// 		}
	// 		// Empty
	// 		else
	// 			continue;
	// 	}
	// }

	// Add horizontal symbols
	horizontalSymbolAdd(CROSS_SYMBOL, 8);

	// Add vertical symbols
	verticalSymbolAdd(EQUAL_SYMBOL, 10);
	verticalSymbolAdd(CROSS_SYMBOL, 12);
	verticalSymbolAdd(EQUAL_SYMBOL, 21);

	// Add pieces
	startingTileAdd(SUN_TILE, 5);
	startingTileAdd(SUN_TILE, 11);
	startingTileAdd(SUN_TILE, 19);
	startingTileAdd(SUN_TILE, 22);
	startingTileAdd(MOON_TILE, 20);
	startingTileAdd(MOON_TILE, 24);

	solve();

	cleanup();
	return 0;
}