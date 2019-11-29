#pragma once
#include "define.h"
#include <vector>

int checkTile(int[][WIDTH]);

int checkSpace(int[][WIDTH]);

int checkSection(int[][WIDTH]);

int checkAllStage(int[][WIDTH]);

int checkCarryInSquare(int [][WIDTH], int , int );

void checkCarryInArea(int [][WIDTH], int [][WIDTH]);

int checkNeighborhoodWall(int [][WIDTH], int , int );

int checkCornerSquare(int [][WIDTH], int , int );

void checkPutBox(int [][WIDTH], int [][WIDTH]);

int checkDeadlock(int [][WIDTH],int,int);

int checkChecklist(int[][WIDTH]);

int checkClear(int[][WIDTH]);

int checkPassingList(std::vector<SQUARE>, SQUARE );

int checkStageList(int [][WIDTH], std::vector<SQUARE> , std::vector<SQUARE>);