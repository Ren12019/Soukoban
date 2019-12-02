#pragma once
#include "define.h"

class Grid
{
public:
	Grid();//コンストラクタ
	~Grid();//デストラクタ

	int grid[GRID_SIZE][GRID_SIZE] = {};//ステージのもととなるグリッド
	void setGrid();//乱数でグリッド内の配置を決定する
	void setGridTemplate();//テンプレートの配置を利用しグリッドを作成する
private:
	void rotateGrid();//グリッドを回転させる
	void flipGrid();//リッドを反転させる
	void printGrid();//グリッドを表示する
};