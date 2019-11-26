#include "search.h"
#include <vector>
#include <queue>
#include "define.h"

using namespace std; 
using TargetStage = vector<vector<SQUARE>>;

SQUARE searchBox(int stage[][WIDTH]) {
	SQUARE point_box;
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			if (stage[y][x] == BOX) {
				point_box.x = x;
				point_box.y = y;
				return point_box;
			}
		}
	}
}

int searchBreadthFirst(int stage[][WIDTH]) {
	int search_stage[HEIGHT][WIDTH] = {};
	TargetStage Target;
	SQUARE point_box;

	//init
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			search_stage[y][x] == stage[y][x];
		}
	}
	point_box = searchBox(search_stage);
	 //create Graph

	return 0;
}