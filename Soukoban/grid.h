#pragma once
#include "define.h"

class Grid
{
public:
	Grid();
	~Grid();

	int grid[GRID_SIZE][GRID_SIZE] = {};
	void setGrid();
	void setGridTemplate();
private:
	void rotateGrid();
	void flipGrid();
	void printGrid();
};