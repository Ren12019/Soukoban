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
#include "stage.h"

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

std::vector<SQUARE> searchBoxTwo(int stage[][WIDTH]) {
	std::vector<SQUARE>box_pos(NUMBER_OF_BOX);

	for (int y = 0,cnt_box=0; y < HEIGHT && cnt_box < NUMBER_OF_BOX; y++) {
		for (int x = 0; x < WIDTH; x++) {
			if (stage[y][x] == BOX || stage[y][x] == BOX_ON_GOAL) {
				box_pos[cnt_box].x = x;
				box_pos[cnt_box].y = y;
				cnt_box++;
			}
		}
	}

	return box_pos;
}

std::vector<SQUARE> searchGoalTwo(int stage[][WIDTH]) {
	std::vector<SQUARE>goal_pos(NUMBER_OF_BOX);

	for (int y = 0, cnt_goal = 0; y < HEIGHT && cnt_goal < NUMBER_OF_BOX; y++) {
		for (int x = 0; x < WIDTH; x++) {
			if (stage[y][x] == GOAL || stage[y][x] == BOX_ON_GOAL) {
				goal_pos[cnt_goal].x = x;
				goal_pos[cnt_goal].y = y;
				cnt_goal++;
			}
		}
	}

	return goal_pos;
}

int searchBreadthFirstTwo(int stage[][WIDTH]) {
	using namespace std;
	int search_stage[HEIGHT][WIDTH] = {};//探索用のステージ	
	vector<SQUARE> start_pos(NUMBER_OF_BOX);//荷物の初期位置
	vector<SQUARE> goal_pos(NUMBER_OF_BOX);//荷物の初期位置
	vector<SQUARE> movable_pos(NUMBER_OF_BOX);//収納用
	vector<SQUARE> current_pos(NUMBER_OF_BOX);//荷物の現在座標
	queue<vector<SQUARE>> search_que;//移動可能であり未到達の座標を保存（探索用キュー）
	//vector<vector<vector<int>>> dist(NUMBER_OF_BOX, vector< vector<int>>(HEIGHT, vector<int>(WIDTH, -1)));//通過した座標のスタートからの距離
	// 探索中に各マスはどのマスから来たのかを表す配列 (最短経路長を知るだけなら、これは不要)
	//vector<vector<vector<int>>> prev_x(NUMBER_OF_BOX, vector< vector<int>>(HEIGHT, vector<int>(WIDTH, -1)));
	//vector<vector<vector<int>>> prev_y(NUMBER_OF_BOX, vector< vector<int>>(HEIGHT, vector<int>(WIDTH, -1)));
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
	start_pos = searchBoxTwo(stage);
	goal_pos = searchGoalTwo(stage);

	///*スタートの距離を「0」に設定*/
	//for (int cnt_box = 0; cnt_box < NUMBER_OF_BOX; cnt_box++) {
	//	dist[cnt_box][start_pos[cnt_box].y][start_pos[cnt_box].x] = 0;
	//}
	/*スタートをプッシュ*/
	search_que.push(start_pos);
	current_pos = search_que.front();

	//
	/*探索*/
	//
	while (!search_que.empty())
	{
		/*移動（盤面の変更）*/
		for (int cnt_box = 0; cnt_box < NUMBER_OF_BOX; cnt_box++) {
			//現在地が荷物
			if (search_stage[current_pos[cnt_box].y][current_pos[cnt_box].x] == BOX) {
				if (search_stage[search_que.front()[cnt_box].y][search_que.front()[cnt_box].x] == PATH) {
					//移動先が空き
					search_stage[search_que.front()[cnt_box].y][search_que.front()[cnt_box].x] = BOX;
					//移動前に居た場所を空きに
					search_stage[current_pos[cnt_box].y][current_pos[cnt_box].x] = PATH;
					addStageList(search_stage);
				}
				else if (search_stage[search_que.front()[cnt_box].y][search_que.front()[cnt_box].x] == GOAL) {
					//移動先がゴール
					search_stage[search_que.front()[cnt_box].y][search_que.front()[cnt_box].x] = BOX_ON_GOAL;
					//移動前に居た場所を空きに
					search_stage[current_pos[cnt_box].y][current_pos[cnt_box].x] = PATH;
					addStageList(search_stage);
				}
				else if (search_stage[search_que.front()[cnt_box].y][search_que.front()[cnt_box].x] == BOX) {
					continue;
				}
				else {
					cout << "error" << endl;
					cout << search_stage[search_que.front()[cnt_box].y][search_que.front()[cnt_box].x] << "," << search_que.front()[cnt_box].x << "," << search_que.front()[cnt_box].y << endl;
				}
			}
			else if (search_stage[current_pos[cnt_box].y][current_pos[cnt_box].x] == BOX_ON_GOAL) {
				if (search_stage[search_que.front()[cnt_box].y][search_que.front()[cnt_box].x] == PATH) {
					//移動先が空き
					search_stage[search_que.front()[cnt_box].y][search_que.front()[cnt_box].x] = BOX;
					//移動前に居た場所をゴールに
					search_stage[current_pos[cnt_box].y][current_pos[cnt_box].x] = GOAL;
					addStageList(search_stage);
				}
				else if (search_stage[search_que.front()[cnt_box].y][search_que.front()[cnt_box].x] == GOAL) {
					//移動先がゴール
					search_stage[search_que.front()[cnt_box].y][search_que.front()[cnt_box].x] = BOX_ON_GOAL;
					//移動前に居た場所をゴールに
					search_stage[current_pos[cnt_box].y][current_pos[cnt_box].x] = GOAL;
					addStageList(search_stage);
				}
				else if (search_stage[search_que.front()[cnt_box].y][search_que.front()[cnt_box].x] == BOX) {
					continue;
				}
				else {
					cout << "error" << endl;
					cout << search_stage[search_que.front()[cnt_box].y][search_que.front()[cnt_box].x] << "," << search_que.front()[cnt_box].x << "," << search_que.front()[cnt_box].y << endl;
				}
			}
		}
		current_pos = search_que.front();
		search_que.pop();

		/*周囲の移動可能な座標をキューに収納*/
		for (int cnt_box = 0; cnt_box < NUMBER_OF_BOX; cnt_box++) {
			for (int direction = 0; direction < 4; direction++) {
				int next_x = current_pos[cnt_box].x + dx[direction];
				int next_y = current_pos[cnt_box].y + dy[direction];
				// 場外検知
				if (next_x < 0 || next_x >= WIDTH || next_y < 0 || next_y >= HEIGHT) continue;
				//荷物が移動可能か
				if (search_stage[next_y][next_x] == PATH || search_stage[next_y][next_x] == GOAL) {
					movable_pos[cnt_box].x = next_x;
					movable_pos[cnt_box].y = next_y;
					//履歴にない
					if (!checkStageList(search_stage,current_pos,movable_pos)) {//ダメっぽい？
						search_que.push(movable_pos);
						//dist[cnt_box][movable_pos[cnt_box].y][movable_pos[cnt_box].x] = dist[cnt_box][current_pos[cnt_box].y][current_pos[cnt_box].x] + 1;
						//prev_x[cnt_box][movable_pos[cnt_box].y][movable_pos[cnt_box].x] = current_pos[cnt_box].x;  // どの頂点から情報が伝播して来たか、縦方向の座標をメモ
						//prev_y[cnt_box][movable_pos[cnt_box].y][movable_pos[cnt_box].x] = current_pos[cnt_box].y;  // どの頂点から情報が伝播して来たか、横方向の座標をメモ
					}
				}
			}
		}

		/*探索の終了確認*/
		if (checkClear(search_stage)) {
			cout << "Clear" << endl;
			break;
		}
		if (search_que.empty()) {
			cout << "Not found route" << endl;
			//return 0;
		}
	}
	printStage(search_stage);
	//
	/* 結果出力 */
	//
	/* 各マスへのスタートからの最短距離を見てみる */
	/*for (int cnt_box = 0; cnt_box < NUMBER_OF_BOX; cnt_box++) {
		cout << cnt_box << endl;
		for (int y = 0; y < HEIGHT; ++y) {
			for (int x = 0; x < WIDTH; ++x) {
				if (stage[y][x] == WALL) cout << "###";
				else cout << setw(3) << dist[cnt_box][y][x];
			}
			cout << endl;
		}
		cout << endl;
	}*/

	/* ゴールに至るまでの最短経路を復元してみる */
	//for (int cnt_box = 0; cnt_box < NUMBER_OF_BOX; cnt_box++) {
	//	int x = goal_pos[cnt_box].x, y = goal_pos[cnt_box].y;
	//	while (x != -1 && y != -1) {
	//		search_stage[y][x] = CHECK; // 通過したことを示す

	//		// 前の頂点へ行く
	//		int px = prev_x[cnt_box][y][x];
	//		int py = prev_y[cnt_box][y][x];
	//		x = px, y = py;
	//	}
	//}

	//for (int cnt_box = 0; cnt_box < NUMBER_OF_BOX; cnt_box++) {
	//	search_stage[start_pos[cnt_box].y][start_pos[cnt_box].x] = BOX;
	//	search_stage[goal_pos[cnt_box].y][goal_pos[cnt_box].x] = GOAL;
	//}
	//printStage(search_stage);

	return 1;
}