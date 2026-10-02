#pragma once
#include <stdint.h>

void startingTileAdd(int type, int b);
void horizontalSymbolAdd(int type, int b);
void verticalSymbolAdd(int type, int b);
int startingTileContains(int b);
int isValid(int type, int b);
int placePiece(int type, int b);
int removePiece(int b);
