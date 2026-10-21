#pragma once
#include <stdint.h>

extern uint64_t bbs[12];

void startingTileAdd(int type, int b);
void horizontalSymbolAdd(int type, int b);
void verticalSymbolAdd(int type, int b);
int getTile(int b);
int getLeftTile(int b);
int getRightTile(int b);
int getAboveTile(int b);
int getBelowTile(int b);
int getLeftSymbol(int b);
int getRightSymbol(int b);
int getAboveSymbol(int b);
int getBelowSymbol(int b);
int startingTileContains(int b);
int isValid(int type, int b);
int placePiece(int type, int b);
int removePiece(int b);
void printBoard();
void updateBitBoardList();