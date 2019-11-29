#include "search.h"
#include <iostream>
#include <vector>
#include <queue>
#include <iomanip>
#include <string> 
#include "define.h"
#include "count.h"
#include "print.h"
#include "check.h"

/* 4 方向への隣接頂点への移動を表すベクトル */
const int dx[4] = { 1, 0, -1, 0 };//右、下、左、上
const int dy[4] = { 0, 1, 0, -1 };//右、下、左、上

SQUARE searchBox(int stage[][WIDTH]) {
	SQUARE point_box;
	point_box.x = 0;
	point_box.y = 0;
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			if (stage[y][x] == BOX) {
				point_box.x = x;
				point_box.y = y;
				return point_box;
			}
		}
	}

	return point_box;
}

SQUARE searchGoal(int stage[][WIDTH]) {
	SQUARE point_goal;
	point_goal.x = 0;
	point_goal.y = 0;
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			if (stage[y][x] == GOAL) {
				point_goal.x = x;
				point_goal.y = y;
				return point_goal;
			}
		}
	}

	return point_goal;
}

int searchBreadthFirst(int stage[][WIDTH]) {
	int search_stage[HEIGHT][WIDTH] = {};//探索用のステージ	
	SQUARE start_pos;//荷物の初期位置
	SQUARE goal_pos;//荷物の初期位置
	SQUARE movable_pos;//収納用
	SQUARE current_pos;//荷物の現在座標
	std::queue<SQUARE> search;//移動可能であり未到達の座標を保存（探索用キュー）
	std::vector<std::vector<int>> dist(HEIGHT, std::vector<int>(WIDTH,-1));//通過した座標のスタートからの距離
	// 探索中に各マスはどのマスから来たのかを表す配列 (最短経路長を知るだけなら、これは不要)
	std::vector<std::vector<int> > prev_x(HEIGHT, std::vector<int>(WIDTH, -1));
	std::vector<std::vector<int> > prev_y(HEIGHT, std::vector<int>(WIDTH, -1));
	                    /*(縦のサイズ,(横のサイズ,初期化する値))*/

	//
	/*初期設定*/
	//
	/*探索用ステージの初期化*/
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			search_stage[y][x] = stage[y][x];
		}
	}
	/*初期位置をセット*/
	start_pos = searchBox(stage);
	goal_pos = searchGoal(stage);
	/*スタートの距離を「0」に設定*/
	dist[start_pos.y][start_pos.x] = 0;
	/*スタートをプッシュ*/
	search.push(start_pos);
	current_pos = search.front();


	//
	/*探索*/
	//
	while (!search.empty())
	{
		/*移動（盤面の変更）*/
		search_stage[current_pos.y][current_pos.x] = PATH;
		if (search_stage[search.front().y][search.front().x] == PATH) {
			search_stage[search.front().y][search.front().x] = BOX;
		}
		else if (search_stage[search.front().y][search.front().x] == GOAL) {
			search_stage[search.front().y][search.front().x] = BOX_ON_GOAL;
		}
		current_pos = search.front();
		search.pop();

		/*周囲の移動可能な座標をキューに収納*/
		for (int direction = 0; direction < 4; direction++) {
			int next_x = current_pos.x + dx[direction];
			int next_y = current_pos.y + dy[direction];
			// 場外検知
			if (next_x < 0 || next_x >= WIDTH || next_y < 0 || next_y >= HEIGHT) continue; 
			//荷物が移動可能か
			if (search_stage[next_y][next_x] != WALL && search_stage[next_y][next_x] != BOX && search_stage[next_y][next_x] != MAN) {
				movable_pos.x = next_x;
				movable_pos.y = next_y;
				//通過済みか確認
				if (dist[movable_pos.y][movable_pos.x] == -1) {
					search.push(movable_pos);
					dist[movable_pos.y][movable_pos.x] = dist[current_pos.y][current_pos.x] + 1;
					prev_x[movable_pos.y][movable_pos.x] = current_pos.x;  // どの頂点から情報が伝播して来たか、縦方向の座標をメモ
					prev_y[movable_pos.y][movable_pos.x] = current_pos.y;  // どの頂点から情報が伝播して来たか、横方向の座標をメモ
				}
			}
		}

		/*探索の終了確認*/
		if (checkClear(search_stage)) {
			std::cout << "Clear" << std::endl;
			printStage(search_stage);
			break;
		}
		if (search.empty()) {
			std::cout << "Not found route" << std::endl;
			return 0;
		}
	}
	
	//
	/* 結果出力 */
	//
	/* 各マスへのスタートからの最短距離を見てみる */
	for (int y = 0; y < HEIGHT; ++y) {
		for (int x = 0; x < WIDTH; ++x) {
			if (stage[y][x] == WALL) std::cout << "###";
			else std::cout << std::setw(3) << dist[y][x];
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;

	/* ゴールに至るまでの最短経路を復元してみる */
	int x = goal_pos.x, y = goal_pos.y;
	while (x != -1 && y != -1) {
		search_stage[y][x] = CHECK; // 通過したことを示す

		// 前の頂点へ行く
		int px = prev_x[y][x];
		int py = prev_y[y][x];
		x = px, y = py;
	}

	search_stage[start_pos.y][start_pos.x] = BOX;
	search_stage[goal_pos.y][goal_pos.x] = GOAL;
	printStage(search_stage);

	return 1;
}