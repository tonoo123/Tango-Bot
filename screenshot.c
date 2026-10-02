#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "screenshot.h"

HWND hwndScreen;
HDC hdcScreen;
HDC hdcMem;

int width;
int height;

HBITMAP hBitmap;
BITMAPINFO bmi;
unsigned char *pixelBuf;

void initHandles_Bitmap()
{
	hwndScreen = GetDesktopWindow();
	hdcScreen = GetDC(hwndScreen);
	hdcMem = CreateCompatibleDC(hdcScreen);
	width = GetSystemMetrics(SM_CXSCREEN);
	height = GetSystemMetrics(SM_CYSCREEN);
	hBitmap = CreateCompatibleBitmap(hdcScreen, width, height);
	SelectObject(hdcMem, hBitmap);
	// bitmap format
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = width;
	bmi.bmiHeader.biHeight = height; // Positive height means bottom-to-top layout
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 32;   // 4 bytes per pixel (B, G, R, A)
	bmi.bmiHeader.biCompression = BI_RGB;
}

unsigned char* capture()
{
	BitBlt(hdcMem, 0, 0, width, height, hdcScreen, 0, 0, SRCCOPY);
	int bufSize = width * height * 4;
	pixelBuf = (unsigned char*)malloc(bufSize);
	GetDIBits(hdcScreen, hBitmap, 0, height, pixelBuf, &bmi, DIB_RGB_COLORS);
	return pixelBuf;
}

void cleanup()
{
	free(pixelBuf);
	DeleteObject(hBitmap);
	DeleteDC(hdcMem);
	ReleaseDC(hwndScreen, hdcScreen);
}