#include "search.h"
#include <vector>
#include <queue>
#include "define.h"
#include "count.h"

using namespace std; 

typedef struct edge {
	SQUARE to;
	int weight;
	struct edge(SQUARE t, int w) : to(t), weight(w) { };
}EDGE;

using TargetStage = vector<vector<EDGE>>;

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

//EDGE searchAround(int stage[][WIDTH]) {
//	EDGE a;
//
//	return ;
//}

int searchBreadthFirst(int stage[][WIDTH]) {
	int search_stage[HEIGHT][WIDTH] = {};
	TargetStage Target(countPath(stage));
	SQUARE point_box;

	//init
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			search_stage[y][x] = stage[y][x];
		}
	}
	point_box = searchBox(search_stage);
	 //create Graph


	return 0;
}