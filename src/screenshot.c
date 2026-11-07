#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "screenshot.h"

HWND hwndScreen;
HDC hdcScreen;
HDC hdcMem;

static int width;
static int height;

HBITMAP hBitmap;
BITMAPINFO bmi;
unsigned char *pixelBuf;

void initHandles_Bitmap(int w, int h)
{
	hwndScreen = GetDesktopWindow();
	hdcScreen = GetDC(hwndScreen);
	hdcMem = CreateCompatibleDC(hdcScreen);
	width = w;
	height = h;
	hBitmap = CreateCompatibleBitmap(hdcScreen, width, height);
	SelectObject(hdcMem, hBitmap);
	// bitmap format
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = width;
	bmi.bmiHeader.biHeight = -height; // Positive height means bottom-to-top layout
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 32;   // 4 bytes per pixel (B, G, R, A)
	bmi.bmiHeader.biCompression = BI_RGB;
}

unsigned char* capture(int screenx, int screeny)
{
	// args: handle, screen x, screen y, width, height, handle, dest x, dest y, raster-op
	BitBlt(hdcMem, screenx, screeny, width, height, hdcScreen, 0, 0, SRCCOPY);
	int bufSize = width * height * 4;
	pixelBuf = (unsigned char*)malloc(bufSize);
	GetDIBits(hdcMem, hBitmap, 0, height, pixelBuf, &bmi, DIB_RGB_COLORS);
	return pixelBuf;
}

void cleanup()
{
	free(pixelBuf);
	DeleteObject(hBitmap);
	DeleteDC(hdcMem);
	ReleaseDC(hwndScreen, hdcScreen);
}