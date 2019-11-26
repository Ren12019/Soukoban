#include "search.h"
#include <vector>
#include <queue>
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

const int dx[4] = { 0, 1, 0, -1 };
const int dy[4] = { 1, 0, -1, 0 };

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

int searchBreadthFirst(int stage[][WIDTH]) {
	////////////////////////////////////////
	/* 準備 */
	////////////////////////////////////////
	SQUARE position_box;
	SQUARE position_goal;
	//int N = countSpace(stage);//頂点の数
	//int M = countConnectionAll(stage);//辺の数
	//TargetStage Target(N);

	 /* 盤面 */
	/*vector<int> field(WIDTH);
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			field[(y*WIDTH) + x] = stage[y][x];
		}
	}*/

	/* スタート地点とゴール地点 */
	position_box = searchBox(stage);
	position_goal = searchGoal(stage);
	////////////////////////////////////////
	/* 幅優先探索の初期設定 */
	////////////////////////////////////////
	// 各セルの最短距離 (訪れていないところは -1 としておく)
	vector<vector<int> > dist(HEIGHT, vector<int>(WIDTH, -1));
	dist[position_box.y][position_box.x] = 0; // スタートを 0 に

	// 探索中に各マスはどのマスから来たのかを表す配列 (最短経路長を知るだけなら、これは不要)
	vector<vector<int> > prev_x(WIDTH, vector<int>(HEIGHT, -1));
	vector<vector<int> > prev_y(WIDTH, vector<int>(HEIGHT, -1));

	// 「一度見た頂点」のうち「まだ訪れていない頂点」を表すキュー
	queue<pair<int, int> > que;
	que.push(make_pair(position_box.y, position_box.x)); // スタートを push

	////////////////////////////////////////
	/* 幅優先探索を実施 */
	////////////////////////////////////////    

	/* キューが空になるまで */
	while (!que.empty()) {
		pair<int, int> current_pos = que.front(); // キューの先頭を見る (C++ ではこれをしても pop しない)
		int x = current_pos.second;
		int y = current_pos.first;
		que.pop(); // キューから pop を忘れずに

		// 隣接頂点を探索
		for (int direction = 0; direction < 4; ++direction) {
			int next_x = x + dx[direction];
			int next_y = y + dy[direction];
			if (next_x < 0 || next_x >= WIDTH || next_y < 0 || next_y >= HEIGHT) continue; // 場外アウトならダメ
			if (stage[next_y][next_x] == WALL) continue; // 壁はダメ

			// まだ見ていない頂点なら push
			if (dist[next_y][next_x] == -1) {
				que.push(make_pair(next_y, next_x));
				dist[next_y][next_x] = dist[y][x] + 1; // (next_x, next_y) の距離も更新
				prev_x[next_y][next_x] = x;  // どの頂点から情報が伝播して来たか、縦方向の座標をメモ
				prev_y[next_y][next_x] = y;  // どの頂点から情報が伝播して来たか、横方向の座標をメモ
			}
		}
	}

	////////////////////////////////////////
   /* 結果出力 */
   ////////////////////////////////////////    

   /* 各マスへのスタートからの最短距離を見てみる */
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			if (stage[y][x] == WALL) printf("#");
			else printf("%d",dist[y][x]);
		}
		printf("\n");
	}
	printf("\n");

	/* ゴールに至るまでの最短経路を復元してみる */
	int x =position_goal.x, y = position_goal.y;
	while (x != -1 && y != -1) {
		stage[y][x] = CHECK; // 通過したことを示す

		// 前の頂点へ行く
		int px = prev_x[x][y];
		int py = prev_y[x][y];
		x = px, y = py;
	}
	
	printStage(stage);

	return 0;
}