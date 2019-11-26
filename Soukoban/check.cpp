#include "check.h"
#include <stdio.h>
#include "define.h"
#include "count.h"

int checkTile(int stage[][WIDTH])
{
  int flag = 0;

  for (int y = 0; y < HEIGHT; y++)
  {
    for (int x = 0; x < WIDTH; x++)
    {
      if (stage[y][x] == PATH)
      {
        if (countAroundWall(stage, x, y) >= 3)
        {
          stage[y][x] = WALL;
          flag = 1;
        }
      }
    }
  }

  return flag;
}

int checkSpace(int stage[][WIDTH])
{
  int count = 0;
  int flag = 0;
  int space_width = 0;
  int space_height = 0;

  for (int y = 0; y < HEIGHT; y++)
  {
    for (int x = 0; x < WIDTH; x++)
    {
      if (stage[y][x] == PATH)
      {
        for (int v = y, h = x; v < HEIGHT && stage[v][h] == PATH && space_height + space_width != 8; v++, count = 0, space_height++)
        {
          for (h = x; h < WIDTH && stage[v][h] == PATH && space_width != count; h++)
          {
            count++;
          }
          if (space_width == 0 && count >= 3)
            space_width = count;
        }
      }
    }
  }
  if (space_height + space_width == 8)
    flag = 1;

  return flag;
}

int checkSection(int stage[][WIDTH])
{
  int checksheet[HEIGHT][WIDTH] = {};
  int section = 1;
  int flag = 1;

  for (int y = 1; y < HEIGHT - 1; y++)
  {
    for (int x = 1; x < WIDTH - 1; x++)
    {
      if (stage[y][x] == PATH || stage[y][x] == GOAL)
      {
        if (checksheet[y - 1][x] != 0)
          checksheet[y][x] = checksheet[y - 1][x];
        else if (checksheet[y][x - 1] != 0)
          checksheet[y][x] = checksheet[y][x - 1];
        else
        {
          checksheet[y][x] = section;
          section++;
        }
      }
    }
  }

  while (flag)
  {
    flag = 0;

    for (int y = 1; y < HEIGHT - 1; y++)
    {
      for (int x = 1; x < WIDTH - 1; x++)
      {
        if (checksheet[y][x] != 0)
        {
          if (checksheet[y - 1][x] != 0 && checksheet[y][x] > checksheet[y - 1][x])
          {
            checksheet[y][x] = checksheet[y - 1][x];
            flag = 1;
          }
          if (checksheet[y][x - 1] != 0 && checksheet[y][x] > checksheet[y][x - 1])
          {
            checksheet[y][x] = checksheet[y][x - 1];
            flag = 1;
          }
          if (checksheet[y + 1][x] != 0 && checksheet[y][x] > checksheet[y + 1][x])
          {
            checksheet[y][x] = checksheet[y + 1][x];
            flag = 1;
          }
          if (checksheet[y][x + 1] != 0 && checksheet[y][x] > checksheet[y][x + 1])
          {
            checksheet[y][x] = checksheet[y][x + 1];
            flag = 1;
          }
        }
      }
    }
  }

  for (int y = 1; y < HEIGHT - 1; y++)
  {
    for (int x = 1; x < WIDTH - 1; x++)
    {
      if (checksheet[y][x] != 0 && checksheet[y][x] != 1)
      {
        return 1;
      }
    }
  }

  return 0;
}

int checkAllStage(int stage[][WIDTH])
{
	int flag = 0;
	STAGELIST *nowStageList = head;

	if ((head == NULL) && (tail == NULL))
	{
		printf("ƒŠƒXƒg‚Í‹ó‚Å‚·B\n");
		return 0;
	}

	while (true)
	{
		flag = 0;
		for (int y = 0; y < HEIGHT; y++)
		{
			for (int x = 0; x < WIDTH; x++)
			{
				if (stage[y][x] != nowStageList->stage[y][x]) {
					flag = 1;
					break;
				}
			}
			if (flag == 0 && y == HEIGHT - 1)
				return 1;
			if (flag == 1)
				break;
		}

		if (nowStageList->next_stage == NULL)
			break;
		else
			nowStageList = nowStageList->next_stage;
	}

	return 0;
}

int checkCarryInSquare(int stage[][WIDTH],int x,int y) {
	if ((stage[y][x - 1] == PATH || stage[y][x - 1] == GOAL) && (stage[y][x + 1] == PATH || stage[y][x + 1] == GOAL)) {
		if (stage[y][x - 2] == WALL && stage[y][x + 2] == WALL && stage[y - 1][x] == WALL && stage[y + 1][x] == WALL) {
			return 1;
		}
	}
	
	if ((stage[y - 1][x] == PATH || stage[y - 1][x] == GOAL) && (stage[y + 1][x] == PATH || stage[y + 1][x] == GOAL)) {
		if (stage[y - 2][x] == WALL && stage[y + 2][x] == WALL && stage[y][x - 1] == WALL && stage[y][x + 1] == WALL) {
			return 1;
		}
	}

	return 0;
}

void checkCarryInArea(int stage[][WIDTH], int checklist[][WIDTH]) {
	for (int y = 0; y < HEIGHT; y++){
		for (int x = 0; x < WIDTH; x++){
			if (stage[y][x] == PATH) {
				if (checkCarryInSquare(stage, x, y)) {
					checklist[y][x] = CHECK;
				}
				else {
					checklist[y][x] = PATH;
				}
			}
			else if (stage[y][x] == WALL) {
				checklist[y][x] = CHECK;
			}
		}
	}
}

int checkNeighborhoodWall(int stage[][WIDTH], int x, int y) {
	//Left
	if (stage[y - 1][x - 1] == WALL && stage[y][x - 1] == WALL && stage[y + 1][x - 1] == WALL) {
		return 1;
	}
	//Right
	if (stage[y - 1][x + 1] == WALL && stage[y][x + 1] == WALL && stage[y + 1][x + 1] == WALL) {
		return 1;
	}
	//Up
	if (stage[y - 1][x - 1] == WALL && stage[y - 1][x] == WALL && stage[y - 1][x + 1] == WALL) {
		return 1;
	}
	//Down
	if (stage[y + 1][x - 1] == WALL && stage[y + 1][x] == WALL && stage[y + 1][x + 1] == WALL) {
		return 1;
	}

	return 0;
}

int checkCornerSquare(int stage[][WIDTH], int x, int y) {
	//„¯
	if (stage[y][x - 1] == WALL && stage[y - 1][x + 1] == WALL && stage[y + 1][x - 1] == WALL && stage[y + 1][x] == WALL) {
		return 1;
	}
	//„®
	if (stage[y-1][x - 1] == WALL && stage[y][x + 1] == WALL && stage[y + 1][x] == WALL && stage[y + 1][x+1] == WALL) {
		return 1;
	}
	//„¬
	if (stage[y-1][x - 1] == WALL && stage[y - 1][x] == WALL && stage[y][x - 1] == WALL && stage[y + 1][x+1] == WALL) {
		return 1;
	}
	//„­
	if (stage[y-1][x] == WALL && stage[y - 1][x + 1] == WALL && stage[y][x + 1] == WALL && stage[y + 1][x-1] == WALL) {
		return 1;
	}

	return 0;
}

void checkPutBox(int stage[][WIDTH], int checklist[][WIDTH]){
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			if (stage[y][x] == PATH) {
				if (checkNeighborhoodWall(stage, x, y)) {
					checklist[y][x] = CHECK;
				}
				else if (checkCornerSquare(stage, x, y)) {
					checklist[y][x] = CHECK;
				}
				else {
					checklist[y][x] = PATH;
				}
			}
			else if (stage[y][x] == WALL) {
				checklist[y][x] = CHECK;
			}
		}
	}
}

int checkDeadlock(int stage[][WIDTH], int x, int y) {
	/*four boxes*/
	if ((stage[y][x + 1] == BOX || stage[y][x + 1] == BOX_ON_GOAL) && (stage[y + 1][x] == BOX || stage[y + 1][x] == BOX_ON_GOAL) && (stage[y + 1][x + 1] == BOX || stage[y + 1][x + 1] == BOX_ON_GOAL)) {
		return 1;
	}
	else if ((stage[y][x - 1] == BOX || stage[y][x - 1] == BOX_ON_GOAL) && (stage[y + 1][x] == BOX || stage[y + 1][x] == BOX_ON_GOAL) && (stage[y + 1][x - 1] == BOX || stage[y + 1][x - 1] == BOX_ON_GOAL)) {
		return 1;
	}
	else if ((stage[y][x + 1] == BOX || stage[y][x + 1] == BOX_ON_GOAL) && stage[y - 1][x] == BOX && (stage[y - 1][x + 1] == BOX || stage[y - 1][x + 1] == BOX_ON_GOAL)) {
		return 1;
	}
	else if ((stage[y][x - 1] == BOX || stage[y][x - 1] == BOX_ON_GOAL) && stage[y - 1][x] == BOX && (stage[y - 1][x - 1] == BOX || stage[y - 1][x - 1] == BOX_ON_GOAL)) {
		return 1;
	}
	/*one box two wall*/
	else if (stage[y][x + 1] == WALL && stage[y - 1][x] == WALL) {
		return 1;
	}
	else if (stage[y][x - 1] == WALL && stage[y - 1][x] == WALL) {
		return 1;
	}
	else if (stage[y][x - 1] == WALL && stage[y + 1][x] == WALL) {
		return 1;
	}
	else if (stage[y][x + 1] == WALL && stage[y + 1][x] == WALL) {
		return 1;
	}
	/*two box two wall( )*/
	else if ((stage[y][x + 1] == BOX || stage[y][x + 1] == BOX_ON_GOAL) && stage[y - 1][x] == WALL && stage[y - 1][x + 1] == WALL) {
		return 1;
	}
	else if ((stage[y][x - 1] == BOX || stage[y][x - 1] == BOX_ON_GOAL) && stage[y - 1][x] == WALL && stage[y - 1][x - 1] == WALL) {
		return 1;
	}
	else if ((stage[y][x + 1] == BOX || stage[y][x + 1] == BOX_ON_GOAL) && stage[y + 1][x] == WALL && stage[y + 1][x + 1] == WALL) {
		return 1;
	}
	else if ((stage[y][x - 1] == BOX || stage[y][x - 1] == BOX_ON_GOAL) && stage[y + 1][x] == WALL && stage[y + 1][x - 1] == WALL) {
		return 1;
	}
	//
	else if (stage[y - 1][x] == BOX && stage[y][x - 1] == WALL && stage[y - 1][x - 1] == WALL) {
		return 1;
	}
	else if ((stage[y + 1][x] == BOX || stage[y + 1][x] == BOX_ON_GOAL) && stage[y][x - 1] == WALL && stage[y + 1][x - 1] == WALL) {
		return 1;
	}
	else if (stage[y - 1][x] == BOX && stage[y][x + 1] == WALL && stage[y - 1][x + 1] == WALL) {
		return 1;
	}
	else if ((stage[y + 1][x] == BOX || stage[y + 1][x] == BOX_ON_GOAL) && stage[y][x + 1] == WALL && stage[y + 1][x + 1] == WALL) {
		return 1;
	}
	/*two box two wall(S)*/
	else if ((stage[y][x + 1] == BOX || stage[y][x + 1] == BOX_ON_GOAL) && stage[y - 1][x] == WALL && stage[y + 1][x + 1] == WALL) {
		return 1;
	}
	else if ((stage[y][x + 1] == BOX || stage[y][x + 1] == BOX_ON_GOAL) && stage[y + 1][x] == WALL && stage[y - 1][x + 1] == WALL) {
		return 1;
	}
	else if ((stage[y][x - 1] == BOX || stage[y][x - 1] == BOX_ON_GOAL) && stage[y - 1][x] == WALL && stage[y + 1][x - 1] == WALL) {
		return 1;
	}
	else if ((stage[y][x - 1] == BOX || stage[y][x - 1] == BOX_ON_GOAL) && stage[y + 1][x] == WALL && stage[y - 1][x - 1] == WALL) {
		return 1;
	}
	//
	else if ((stage[y + 1][x] == BOX || stage[y + 1][x] == BOX_ON_GOAL) && stage[y][x + 1] == WALL && stage[y + 1][x - 1] == WALL) {
		return 1;
	}
	else if ((stage[y + 1][x] == BOX || stage[y + 1][x] == BOX_ON_GOAL) && stage[y][x - 1] == WALL && stage[y + 1][x + 1] == WALL) {
		return 1;
	}
	else if ((stage[y - 1][x] == BOX || stage[y - 1][x] == BOX_ON_GOAL) && stage[y - 1][x + 1] == WALL && stage[y][x - 1] == WALL) {
		return 1;
	}
	else if ((stage[y - 1][x] == BOX || stage[y - 1][x] == BOX_ON_GOAL) && stage[y - 1][x - 1] == WALL && stage[y][x + 1] == WALL) {
		return 1;
	}
	/*three boxes one wall*/
	else if (stage[y][x + 1] == WALL && (stage[y + 1][x] == BOX || stage[y + 1][x] == BOX_ON_GOAL) && (stage[y + 1][x + 1] == BOX || stage[y + 1][x + 1] == BOX_ON_GOAL)) {
		return 1;
	}
	else if ((stage[y][x + 1] == BOX || stage[y][x + 1] == BOX_ON_GOAL) && stage[y + 1][x] == WALL && (stage[y + 1][x + 1] == BOX || stage[y + 1][x + 1] == BOX_ON_GOAL)) {
		return 1;
	}
	else if ((stage[y][x + 1] == BOX || stage[y][x + 1] == BOX_ON_GOAL) && (stage[y + 1][x] == BOX || stage[y + 1][x] == BOX_ON_GOAL) && stage[y + 1][x + 1] == WALL) {
		return 1;
	}
	else if (stage[y][x - 1] == WALL && (stage[y + 1][x - 1] == BOX || stage[y + 1][x - 1] == BOX_ON_GOAL) && stage[y + 1][x] == WALL) {
		return 1;
	}
	/*three boxes two wall*/
	//1
	else if ((stage[y][x + 1] == BOX || stage[y][x + 1] == BOX_ON_GOAL) && (stage[y + 1][x + 1] == BOX || stage[y + 1][x + 1] == BOX_ON_GOAL) && stage[y - 1][x] == WALL && stage[y + 1][x + 2] == WALL) {
		return 1;
	}
	else if ((stage[y][x - 1] == BOX || stage[y][x - 1] == BOX_ON_GOAL) && (stage[y + 1][x] == BOX || stage[y + 1][x] == BOX_ON_GOAL) && stage[y - 1][x - 1] == WALL && stage[y + 1][x + 1] == WALL) {
		return 1;
	}
	else if ((stage[y + 1][x] == BOX || stage[y + 1][x] == BOX_ON_GOAL) && (stage[y - 1][x - 1] == BOX || stage[y - 1][x - 1] == BOX_ON_GOAL) && stage[y][x + 1] == WALL && stage[y - 2][x - 1] == WALL) {
		return 1;
	}
	//2
	else if ((stage[y][x + 1] == BOX || stage[y][x + 1] == BOX_ON_GOAL) && (stage[y + 1][x] == BOX || stage[y + 1][x] == BOX_ON_GOAL) && stage[y - 1][x + 1] == WALL && stage[y + 1][x - 1] == WALL) {
		return 1;
	}
	else if ((stage[y][x - 1] == BOX || stage[y][x - 1] == BOX_ON_GOAL) && (stage[y + 1][x - 1] == BOX || stage[y + 1][x - 1] == BOX_ON_GOAL) && stage[y - 1][x] == WALL && stage[y + 1][x - 2] == WALL) {
		return 1;
	}
	else if (stage[y - 1][x] == BOX && (stage[y - 1][x + 1] == BOX || stage[y - 1][x + 1] == BOX_ON_GOAL) && stage[y - 2][x + 1] == WALL && stage[y][x - 1] == WALL) {
		return 1;
	}
	//3
	else if ((stage[y][x + 1] == BOX || stage[y][x + 1] == BOX_ON_GOAL) && (stage[y - 1][x + 1] == BOX || stage[y - 1][x + 1] == BOX_ON_GOAL) && stage[y - 1][x + 2] == WALL && stage[y + 1][x] == WALL) {
		return 1;
	}
	else if ((stage[y + 1][x] == BOX || stage[y + 1][x] == BOX_ON_GOAL) && (stage[y + 1][x - 1] == BOX || stage[y + 1][x - 1] == BOX_ON_GOAL) && stage[y][x + 1] == WALL && stage[y + 2][x - 1] == WALL) {
		return 1;
	}
	else if ((stage[y][x - 1] == BOX || stage[y][x - 1] == BOX_ON_GOAL) && stage[y - 1][x] == BOX && stage[y - 1][x + 1] == WALL && stage[y + 1][x - 1] == WALL) {
		return 1;
	}
	//4
	else if ((stage[y + 1][x] == BOX || stage[y + 1][x] == BOX_ON_GOAL) && (stage[y + 1][x + 1] == BOX || stage[y + 1][x + 1] == BOX_ON_GOAL) && stage[y][x - 1] == WALL && stage[y + 2][x + 1] == WALL) {
		return 1;
	}
	else if (stage[y - 1][x] == BOX && (stage[y][x + 1] == BOX || stage[y][x + 1] == BOX_ON_GOAL) && stage[y - 1][x - 1] == WALL && stage[y + 1][x + 1] == WALL) {
		return 1;
	}
	else if ((stage[y][x - 1] == BOX || stage[y][x - 1] == BOX_ON_GOAL) && (stage[y - 1][x - 1] == BOX || stage[y - 1][x - 1] == BOX_ON_GOAL) && stage[y + 1][x] == WALL && stage[y - 1][x - 2] == WALL) {
		return 1;
	}
	return 0;
}

int checkChecklist(int checklist[][WIDTH]) {
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			if (checklist[y][x] != CHECK) {
				return 0;
			}
		}
	}
	return 1;
}

int checkClear(int stage[][WIDTH]) {
	int cnt_remainder_goal=0;
	int cnt_clear_goal=0;

	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			if (stage[y][x] == GOAL) {
				cnt_remainder_goal++;
			}
			else if (stage[y][x] == BOX_ON_GOAL) {
				cnt_clear_goal++;
			}
		}
	}

	if (cnt_remainder_goal == 0 && cnt_clear_goal == NUMBER_OF_BOX) {
		return 1;
	}

	return 0;
}