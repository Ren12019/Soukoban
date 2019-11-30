#pragma once
#include <vector>
#include "define.h"

void initBox(int [][WIDTH]);

int moveBox(int[][WIDTH],const std::vector<SQUARE>,const std::vector<SQUARE>);