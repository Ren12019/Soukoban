#include "search.h"
#include <vector>
#include <queue>
#include "define.h"

using namespace std; 
using TargetStage = vector<vector<SQUARE>>;


int searchBreadthFirst(int stage[][WIDTH]) {
	int search_stage[HEIGHT][WIDTH] = {};

	//init
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			search_stage[y][x] = stage[y][x];
		}
	}

	return 0;
}