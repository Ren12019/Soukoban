#pragma once
#include "define.h"
#include <vector>

int checkTile(std::vector <std::vector<int>>);

int checkSpace(std::vector <std::vector<int>>);

int checkSection(std::vector <std::vector<int>>);

int checkAllStage(std::vector <std::vector<int>>);

int checkCarryInSquare(std::vector <std::vector<int>>, int , int );

void checkCarryInArea(std::vector <std::vector<int>>, std::vector <std::vector<int>>);

int checkNeighborhoodWall(std::vector <std::vector<int>>, int , int );

int checkCornerSquare(std::vector <std::vector<int>>, int , int );

void checkPutBox(std::vector <std::vector<int>>, std::vector <std::vector<int>>);

int checkDeadlock(std::vector <std::vector<int>>,int,int);

int checkChecklist(std::vector <std::vector<int>>);

int checkClear(std::vector <std::vector<int>>);

int checkPassingList(std::vector<SQUARE>, SQUARE );

int checkStageList(std::vector <std::vector<int>>, std::vector<SQUARE> , std::vector<SQUARE>);