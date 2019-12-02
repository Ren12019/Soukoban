#pragma once
#include "define.h"

class Level
{
public:
	Level();
	~Level();
	int stage[HEIGHT][WIDTH] = {};
	void createLevel();
	void printStage();
	void outputStage();
private:
	void createEmptyRoom();
	void fillBlindAlley();
	bool setGoal();
	bool setBox();
	bool setPlayer();
	bool checkSection();
	int countSpace();
};

void setStageTest(int [][WIDTH], SQUARE);

void set_x(int* );

void set_y(int* );

void initStage(int [][WIDTH]);

void createStageGrid(int [][WIDTH]);

void addStageList(int [][WIDTH]);

void freeStageList();