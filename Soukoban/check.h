#pragma once
#include "define.h"
#include <vector>

int checkTile(int[][WIDTH]);

int checkSpace(const int[][WIDTH]);

int checkSection(const int[][WIDTH]);

int checkAllStage(const int[][WIDTH]);

int checkCarryInSquare(const int [][WIDTH], const int , const int );

void checkCarryInArea(const int [][WIDTH],int [][WIDTH]);

int checkNeighborhoodWall(const int [][WIDTH], const int , const int );

int checkCornerSquare(const int [][WIDTH], const int , const int );

void checkPutBox(const int [][WIDTH],int [][WIDTH]);

int checkDeadlock(const int [][WIDTH], const int, const int);

int checkChecklist(const int[][WIDTH]);

int checkClear(const int[][WIDTH]);

int checkPassingList(const std::vector<SQUARE>, SQUARE );

int checkStageList(const int [][WIDTH], const std::vector<SQUARE> , const SQUARE, const int);