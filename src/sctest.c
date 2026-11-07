#include <stdio.h>
#include <windows.h>
#include "screenshot.h"

// Purely to get rid of VScode undefined error, code will run perfectly fine without it
#ifndef DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2
    DECLARE_HANDLE(DPI_AWARENESS_CONTEXT);
    #define DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2 ((DPI_AWARENESS_CONTEXT)-4)
#endif

int width = 0;
int height = 0;

int topLeft[2] = {743, 390};

int getPixelIndex(int x, int y)
{
	// Pixels have RGBA, so multiply by 4
	return (y*width + x) * 4;
}

int main()
{
	SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
	unsigned char *buf;
	width = GetSystemMetrics(SM_CXSCREEN);
	height = GetSystemMetrics(SM_CYSCREEN);
	printf("width: %d\theight: %d\n", width, height);

	initHandles_Bitmap(width, height);
	buf = capture(0, 0);
	int ind = getPixelIndex(topLeft[0], topLeft[1]);
	printf("index: %d\n", ind);
	printf("%d %d %d %d\n", buf[ind], buf[ind+1], buf[ind+2], buf[ind+3]);
	cleanup();
	return 0;
}