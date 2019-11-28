#include "search.h"
#include <iostream>
#include <vector>
#include <queue>
#include <iomanip>
#include <string> 
#include "define.h"
#include "count.h"
#include "print.h"

using namespace std; 

typedef struct edge {
	SQUARE to;
	int weight;
	struct edge(SQUARE t, int w) : to(t), weight(w) { };
}EDGE;

using TargetStage = vector<vector<EDGE>>;

/* 4 方向への隣接頂点への移動を表すベクトル */
const int dx[4] = { 1, 0, -1, 0 };
const int dy[4] = { 0, 1, 0, -1 };

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

int searchBreadthFirstSample(int stage[][WIDTH]) {
	////////////////////////////////////////
/* 入力受け取り */
////////////////////////////////////////

/* 縦と横の長さ */
	int height, width;
	cout << "幅と高さを入力してください" << endl;
	cin >> height >> width;

	/* 盤面 */
	vector<string> field(height);
	cout << "盤面を一行づつ入力してください" << endl;
	for (int h = 0; h < height; ++h) {
		cout << h << endl;
		cin >> field[h];
	}

	/* スタート地点とゴール地点 */
	int sx, sy, gx, gy;
	for (int h = 0; h < height; ++h) {
		for (int w = 0; w < width; ++w) {
			if (field[h][w] == 'S') {
				sx = h;
				sy = w;
			}
			if (field[h][w] == 'G') {
				gx = h;
				gy = w;
			}
		}
	}


	////////////////////////////////////////
	/* 幅優先探索の初期設定 */
	////////////////////////////////////////

	// 各セルの最短距離 (訪れていないところは -1 としておく)
	vector<vector<int> > dist(height, vector<int>(width, -1));
	dist[sx][sy] = 0; // スタートを 0 に

	// 「一度見た頂点」のうち「まだ訪れていない頂点」を表すキュー
	queue<pair<int, int> > que;
	que.push(make_pair(sx, sy)); // スタートを push


	////////////////////////////////////////
	/* 幅優先探索を実施 */
	////////////////////////////////////////    

	/* キューが空になるまで */
	while (!que.empty()) {
		pair<int, int> current_pos = que.front(); // キューの先頭を見る (C++ ではこれをしても pop しない)
		int x = current_pos.first;
		int y = current_pos.second;
		que.pop(); // キューから pop を忘れずに

		// 隣接頂点を探索
		for (int direction = 0; direction < 4; ++direction) {
			int next_x = x + dx[direction];
			int next_y = y + dy[direction];
			if (next_x < 0 || next_x >= height || next_y < 0 || next_y >= width) continue; // 場外アウトならダメ
			if (field[next_x][next_y] == '#') continue; // 壁はダメ

			// まだ見ていない頂点なら push
			if (dist[next_x][next_y] == -1) {
				que.push(make_pair(next_x, next_y));
				dist[next_x][next_y] = dist[x][y] + 1;
			}
		}
	}


	////////////////////////////////////////
	/* 結果出力 */
	////////////////////////////////////////    

	/* 各マスへのスタートからの最短距離を見てみる */
	for (int h = 0; h < height; ++h) {
		for (int w = 0; w < width; ++w) {
			if (field[h][w] != '.' && field[h][w] != 'G') cout << std::setw(3) << field[h][w];
			else cout << std::setw(3) << dist[h][w];
		}
		cout << endl;
	}
	cout << endl;

	//////////////////////////////////////////
	///* 準備 */
	//////////////////////////////////////////
	//SQUARE position_box;
	//SQUARE position_goal;
	////int N = countSpace(stage);//頂点の数
	////int M = countConnectionAll(stage);//辺の数
	////TargetStage Target(N);

	// /* 盤面 */
	//vector<vector<string>> field(WIDTH);
	///*for (int y = 0; y < HEIGHT; y++) {
	//	for (int x = 0; x < WIDTH; x++) {
	//		if(stage[y][x]==WALL)
	//			field[x][y] = '#';
	//		else if (stage[y][x] == MAN)
	//			field[x][y] = '@';
	//		else if (stage[y][x] == MAN_ON_GOAL)
	//			field[x][y] = '+';
	//		else if (stage[y][x] == BOX)
	//			field[x][y] = '$';
	//		else if (stage[y][x] == BOX_ON_GOAL)
	//			field[x][y] = '*';
	//		else if (stage[y][x] == GOAL)
	//			field[x][y] = '.';
	//		else if (stage[y][x] == PATH)
	//			field[x][y] = ' ';
	//		else if (stage[y][x] == CHECK)
	//			field[x][y] = 'x';
	//	}
	//}*/
	//for (int h = 0; h < HEIGHT; ++h) cin >> field[h];


	///* スタート地点とゴール地点 */
	//position_box = searchBox(stage);
	//position_goal = searchGoal(stage);
	//int sx, sy, gx, gy;
	//sx = position_box.y;
	//sy = position_box.x;
	//gx = position_goal.y;
	//gy = position_goal.x;
	//////////////////////////////////////////
	///* 幅優先探索の初期設定 */
	//////////////////////////////////////////
	//// 各セルの最短距離 (訪れていないところは -1 としておく)
	//vector<vector<int> > dist(HEIGHT, vector<int>(WIDTH, -1));
	//dist[sx][sy] = 0; // スタートを 0 に

	//// 探索中に各マスはどのマスから来たのかを表す配列 (最短経路長を知るだけなら、これは不要)
	//vector<vector<int> > prev_x(WIDTH, vector<int>(HEIGHT, -1));
	//vector<vector<int> > prev_y(WIDTH, vector<int>(HEIGHT, -1));

	//// 「一度見た頂点」のうち「まだ訪れていない頂点」を表すキュー
	//queue<pair<int, int> > que;
	//que.push(make_pair(sx, sy)); // スタートを push

	//////////////////////////////////////////
	///* 幅優先探索を実施 */
	//////////////////////////////////////////    

	// /* キューが空になるまで */
	//while (!que.empty()) {
	//	pair<int, int> current_pos = que.front(); // キューの先頭を見る (C++ ではこれをしても pop しない)
	//	int x = current_pos.first;
	//	int y = current_pos.second;
	//	que.pop(); // キューから pop を忘れずに

	//	// 隣接頂点を探索
	//	for (int direction = 0; direction < 4; ++direction) {
	//		int next_x = x + dx[direction];
	//		int next_y = y + dy[direction];
	//		if (next_x < 0 || next_x >= HEIGHT || next_y < 0 || next_y >= WIDTH) continue; // 場外アウトならダメ
	//		if (field[next_x][next_y] == "#") continue; // 壁はダメ

	//		// まだ見ていない頂点なら push
	//		if (dist[next_x][next_y] == -1) {
	//			que.push(make_pair(next_x, next_y));
	//			dist[next_x][next_y] = dist[x][y] + 1; // (next_x, next_y) の距離も更新
	//			prev_x[next_x][next_y] = x;  // どの頂点から情報が伝播して来たか、縦方向の座標をメモ
	//			prev_y[next_x][next_y] = y;  // どの頂点から情報が伝播して来たか、横方向の座標をメモ
	//		}
	//	}
	//}


	//////////////////////////////////////////
 //  /* 結果出力 */
 //  ////////////////////////////////////////    

 //  /* 各マスへのスタートからの最短距離を見てみる */
	//for (int h = 0; h < HEIGHT; ++h) {
	//	for (int w = 0; w < WIDTH; ++w) {
	//		if (field[h][w] != '.') cout << std::setw(3) << field[h][w];
	//		else cout << std::setw(3) << dist[h][w];
	//	}
	//	cout << endl;
	//}
	//cout << endl;

	///* ゴールに至るまでの最短経路を復元してみる */
	//int x = gx, y = gy;
	//while (x != -1 && y != -1) {
	//	field[x][y] = 'o'; // 通過したことを示す

	//	// 前の頂点へ行く
	//	int px = prev_x[x][y];
	//	int py = prev_y[x][y];
	//	x = px, y = py;
	//}
	//for (int h = 0; h < HEIGHT; ++h) {
	//	for (int w = 0; w < WIDTH; ++w) {
	//		cout << std::setw(3) << field[h][w];
	//	}
	//	cout << endl;
	//}

	return 0;
}

int searchBreadthFirst(int stage[][WIDTH]) {
	
	return 0;
}