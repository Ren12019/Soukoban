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
			if (stage[y][x] == BOX) {
				point_goal.x = x;
				point_goal.y = y;
				return point_goal;
			}
		}
	}

	return point_goal;
}

//EDGE searchAround(int stage[][WIDTH]) {
//	EDGE a;
//
//	return ;
//}

//int searchBreadthFirstSample(int stage[][WIDTH]) {
//	////////////////////////////////////////
///* 入力受け取り */
//////////////////////////////////////////
//
///* 縦と横の長さ */
//	int height, width;
//	cout << "幅と高さを入力してください" << endl;
//	cin >> height >> width;
//
//	/* 盤面 */
//	vector<string> field(height);
//	cout << "盤面を一行づつ入力してください" << endl;
//	for (int h = 0; h < height; ++h) {
//		cout << h << endl;
//		cin >> field[h];
//	}
//
//	/* スタート地点とゴール地点 */
//	int sx, sy, gx, gy;
//	for (int h = 0; h < height; ++h) {
//		for (int w = 0; w < width; ++w) {
//			if (field[h][w] == 'S') {
//				sx = h;
//				sy = w;
//			}
//			if (field[h][w] == 'G') {
//				gx = h;
//				gy = w;
//			}
//		}
//	}
//
//
//	////////////////////////////////////////
//	/* 幅優先探索の初期設定 */
//	////////////////////////////////////////
//
//	// 各セルの最短距離 (訪れていないところは -1 としておく)
//	vector<vector<int> > dist(height, vector<int>(width, -1));
//	dist[sx][sy] = 0; // スタートを 0 に
//
//	// 「一度見た頂点」のうち「まだ訪れていない頂点」を表すキュー
//	queue<pair<int, int> > que;
//	que.push(make_pair(sx, sy)); // スタートを push
//
//
//	////////////////////////////////////////
//	/* 幅優先探索を実施 */
//	////////////////////////////////////////    
//
//	/* キューが空になるまで */
//	while (!que.empty()) {
//		pair<int, int> current_pos = que.front(); // キューの先頭を見る (C++ ではこれをしても pop しない)
//		int x = current_pos.first;
//		int y = current_pos.second;
//		que.pop(); // キューから pop を忘れずに
//
//		// 隣接頂点を探索
//		for (int direction = 0; direction < 4; ++direction) {
//			int next_x = x + dx[direction];
//			int next_y = y + dy[direction];
//			if (next_x < 0 || next_x >= height || next_y < 0 || next_y >= width) continue; // 場外アウトならダメ
//			if (field[next_x][next_y] == '#') continue; // 壁はダメ
//
//			// まだ見ていない頂点なら push
//			if (dist[next_x][next_y] == -1) {
//				que.push(make_pair(next_x, next_y));
//				dist[next_x][next_y] = dist[x][y] + 1;
//			}
//		}
//	}
//
//
//	////////////////////////////////////////
//	/* 結果出力 */
//	////////////////////////////////////////    
//
//	/* 各マスへのスタートからの最短距離を見てみる */
//	for (int h = 0; h < height; ++h) {
//		for (int w = 0; w < width; ++w) {
//			if (field[h][w] != '.' && field[h][w] != 'G') cout << std::setw(3) << field[h][w];
//			else cout << std::setw(3) << dist[h][w];
//		}
//		cout << endl;
//	}
//	cout << endl;
//
//	//////////////////////////////////////////
//	///* 準備 */
//	//////////////////////////////////////////
//	//SQUARE position_box;
//	//SQUARE position_goal;
//	////int N = countSpace(stage);//頂点の数
//	////int M = countConnectionAll(stage);//辺の数
//	////TargetStage Target(N);
//
//	// /* 盤面 */
//	//vector<vector<string>> field(WIDTH);
//	///*for (int y = 0; y < HEIGHT; y++) {
//	//	for (int x = 0; x < WIDTH; x++) {
//	//		if(stage[y][x]==WALL)
//	//			field[x][y] = '#';
//	//		else if (stage[y][x] == MAN)
//	//			field[x][y] = '@';
//	//		else if (stage[y][x] == MAN_ON_GOAL)
//	//			field[x][y] = '+';
//	//		else if (stage[y][x] == BOX)
//	//			field[x][y] = '$';
//	//		else if (stage[y][x] == BOX_ON_GOAL)
//	//			field[x][y] = '*';
//	//		else if (stage[y][x] == GOAL)
//	//			field[x][y] = '.';
//	//		else if (stage[y][x] == PATH)
//	//			field[x][y] = ' ';
//	//		else if (stage[y][x] == CHECK)
//	//			field[x][y] = 'x';
//	//	}
//	//}*/
//	//for (int h = 0; h < HEIGHT; ++h) cin >> field[h];
//
//
//	///* スタート地点とゴール地点 */
//	//position_box = searchBox(stage);
//	//position_goal = searchGoal(stage);
//	//int sx, sy, gx, gy;
//	//sx = position_box.y;
//	//sy = position_box.x;
//	//gx = position_goal.y;
//	//gy = position_goal.x;
//	//////////////////////////////////////////
//	///* 幅優先探索の初期設定 */
//	//////////////////////////////////////////
//	//// 各セルの最短距離 (訪れていないところは -1 としておく)
//	//vector<vector<int> > dist(HEIGHT, vector<int>(WIDTH, -1));
//	//dist[sx][sy] = 0; // スタートを 0 に
//
//	//// 探索中に各マスはどのマスから来たのかを表す配列 (最短経路長を知るだけなら、これは不要)
//	//vector<vector<int> > prev_x(WIDTH, vector<int>(HEIGHT, -1));
//	//vector<vector<int> > prev_y(WIDTH, vector<int>(HEIGHT, -1));
//
//	//// 「一度見た頂点」のうち「まだ訪れていない頂点」を表すキュー
//	//queue<pair<int, int> > que;
//	//que.push(make_pair(sx, sy)); // スタートを push
//
//	//////////////////////////////////////////
//	///* 幅優先探索を実施 */
//	//////////////////////////////////////////    
//
//	// /* キューが空になるまで */
//	//while (!que.empty()) {
//	//	pair<int, int> current_pos = que.front(); // キューの先頭を見る (C++ ではこれをしても pop しない)
//	//	int x = current_pos.first;
//	//	int y = current_pos.second;
//	//	que.pop(); // キューから pop を忘れずに
//
//	//	// 隣接頂点を探索
//	//	for (int direction = 0; direction < 4; ++direction) {
//	//		int next_x = x + dx[direction];
//	//		int next_y = y + dy[direction];
//	//		if (next_x < 0 || next_x >= HEIGHT || next_y < 0 || next_y >= WIDTH) continue; // 場外アウトならダメ
//	//		if (field[next_x][next_y] == "#") continue; // 壁はダメ
//
//	//		// まだ見ていない頂点なら push
//	//		if (dist[next_x][next_y] == -1) {
//	//			que.push(make_pair(next_x, next_y));
//	//			dist[next_x][next_y] = dist[x][y] + 1; // (next_x, next_y) の距離も更新
//	//			prev_x[next_x][next_y] = x;  // どの頂点から情報が伝播して来たか、縦方向の座標をメモ
//	//			prev_y[next_x][next_y] = y;  // どの頂点から情報が伝播して来たか、横方向の座標をメモ
//	//		}
//	//	}
//	//}
//
//
//	//////////////////////////////////////////
// //  /* 結果出力 */
// //  ////////////////////////////////////////    
//
// //  /* 各マスへのスタートからの最短距離を見てみる */
//	//for (int h = 0; h < HEIGHT; ++h) {
//	//	for (int w = 0; w < WIDTH; ++w) {
//	//		if (field[h][w] != '.') cout << std::setw(3) << field[h][w];
//	//		else cout << std::setw(3) << dist[h][w];
//	//	}
//	//	cout << endl;
//	//}
//	//cout << endl;
//
//	///* ゴールに至るまでの最短経路を復元してみる */
//	//int x = gx, y = gy;
//	//while (x != -1 && y != -1) {
//	//	field[x][y] = 'o'; // 通過したことを示す
//
//	//	// 前の頂点へ行く
//	//	int px = prev_x[x][y];
//	//	int py = prev_y[x][y];
//	//	x = px, y = py;
//	//}
//	//for (int h = 0; h < HEIGHT; ++h) {
//	//	for (int w = 0; w < WIDTH; ++w) {
//	//		cout << std::setw(3) << field[h][w];
//	//	}
//	//	cout << endl;
//	//}
//
//	return 0;
//}

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
			printStage(search_stage);
			break;
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
	std::cout << x << "," << y << std::endl;
	while (x != -1 && y != -1) {
		std::cout << "1:" << x << "," << y << std::endl;

		search_stage[y][x] = CHECK; // 通過したことを示す

		// 前の頂点へ行く
		int px = prev_x[y][x];
		int py = prev_y[y][x];
		x = px, y = py;
		std::cout << "2:" << x << "," << y << std::endl;
	}

	printStage(search_stage);

	return 0;
}