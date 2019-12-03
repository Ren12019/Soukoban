#pragma once
#include <string>
#include "define.h"

class Level
{
public:
	Level();
	~Level();
	int stage[HEIGHT][WIDTH] = {};
	void createLevel();//ステージ生成を行う
	void setStage();//空の部屋に配置物をすべてセットする
	void resetStage();//空の部屋はそのままで配置物をリセットする
	void printStage();//ステージを表示する
	void outputStage();//ステージをテキストファイルとして出力
	std::string outputString();//ステージをstring型で出力
private:
	void createEmptyRoom();//空の部屋を作る
	void fillBlindAlley();//ステージの袋小路をつぶす
	bool setGoal();//ステージにゴールを設置する
	bool setBox();//ステージに荷物を設置する
	bool setPlayer();//ステージにプレイヤーを配置する
	bool checkSection();//部屋が2つ以上になっていないか確認
	int countSpace();//ステージの通過可能部分をカウントする
};