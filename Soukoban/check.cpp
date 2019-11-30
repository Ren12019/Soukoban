#include "check.h"
#include <stdio.h>
#include <vector>
#include <iostream>
#include "define.h"
#include "count.h"
#include "box.h"

int checkTile(std::vector <std::vector<int>> stage)
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

int checkSpace(std::vector <std::vector<int>> stage)
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

int checkSection(std::vector <std::vector<int>> stage)
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

int checkAllStage(std::vector <std::vector<int>> stage)
{
	int flag = 0;
	STAGELIST *nowStageList = head;

	if ((head == NULL) && (tail == NULL))
	{
		printf("リストは空です。\n");
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

int checkCarryInSquare(std::vector <std::vector<int>> stage,int x,int y) {
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

void checkCarryInArea(std::vector <std::vector<int>> stage, std::vector <std::vector<int>> checklist) {
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

int checkNeighborhoodWall(std::vector <std::vector<int>> stage, int x, int y) {
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

int checkCornerSquare(std::vector <std::vector<int>> stage, int x, int y) {
	//┗
	if (stage[y][x - 1] == WALL && stage[y - 1][x + 1] == WALL && stage[y + 1][x - 1] == WALL && stage[y + 1][x] == WALL) {
		return 1;
	}
	//┛
	if (stage[y-1][x - 1] == WALL && stage[y][x + 1] == WALL && stage[y + 1][x] == WALL && stage[y + 1][x+1] == WALL) {
		return 1;
	}
	//┏
	if (stage[y-1][x - 1] == WALL && stage[y - 1][x] == WALL && stage[y][x - 1] == WALL && stage[y + 1][x+1] == WALL) {
		return 1;
	}
	//┓
	if (stage[y-1][x] == WALL && stage[y - 1][x + 1] == WALL && stage[y][x + 1] == WALL && stage[y + 1][x-1] == WALL) {
		return 1;
	}

	return 0;
}

void checkPutBox(std::vector <std::vector<int>> stage, std::vector <std::vector<int>> checklist){
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

int checkDeadlock(std::vector <std::vector<int>> stage, int x, int y) {
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
	/*two box two wall(□)*/
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

int checkChecklist(std::vector <std::vector<int>> checklist) {
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			if (checklist[y][x] != CHECK) {
				return 0;
			}
		}
	}
	return 1;
}

int checkClear(std::vector <std::vector<int>> stage) {
	int cnt_remainder_box = 0;
	int cnt_remainder_goal=0;
	int cnt_clear_goal=0;

	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			if (stage[y][x] == BOX) {
				cnt_remainder_box++;
			}
			else if (stage[y][x] == GOAL) {
				cnt_remainder_goal++;
			}
			else if (stage[y][x] == BOX_ON_GOAL) {
				cnt_clear_goal++;
			}
		}
	}

	if (cnt_remainder_box == 0 && cnt_remainder_goal == 0 && cnt_clear_goal == NUMBER_OF_BOX) {
		return 1;
	}

	return 0;
}

int checkPassingList(std::vector<SQUARE>passing_list,SQUARE square) {
	for (int i = 0; i < passing_list.size(); i++) {
		if (passing_list[i].x == square.x && passing_list[i].y == square.y) {
			return 1;
		}
	}

	return 0;
}
/*移動候補が履歴にある盤面なら「１」を返す*/
int checkStageList(std::vector <std::vector<int>> stage, std::vector<SQUARE> current_pos,std::vector<SQUARE> next) {
	using namespace std;
	STAGELIST *nowStageList = head;
	vector <vector<int>> target_stage(HEIGHT,vector<int>(WIDTH, 0));
	vector <vector<int>> check_list(HEIGHT,vector<int>(WIDTH, 0));
	int flag_check = 1;

	/*初期設定*/
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			target_stage[y][x] = stage[y][x];
		}
	}

	/*仮想移動（盤面の変更）*/
	moveBox(target_stage, current_pos, next);
	//for (int cnt_box = 0; cnt_box < NUMBER_OF_BOX; cnt_box++) {
	//	//現在地が荷物
	//	if (target_stage[current_pos[cnt_box].y][current_pos[cnt_box].x] == BOX) {
	//		if (target_stage[next[cnt_box].y][next[cnt_box].x] == PATH) {
	//			//移動先が空き
	//			target_stage[next[cnt_box].y][next[cnt_box].x] = BOX;
	//			//移動前に居た場所を空きに
	//			target_stage[current_pos[cnt_box].y][current_pos[cnt_box].x] = PATH;
	//		}
	//		else if (target_stage[next[cnt_box].y][next[cnt_box].x] == GOAL) {
	//			//移動先がゴール
	//			target_stage[next[cnt_box].y][next[cnt_box].x] = BOX_ON_GOAL;
	//			//移動前に居た場所を空きに
	//			target_stage[current_pos[cnt_box].y][current_pos[cnt_box].x] = PATH;
	//		}
	//		else if (target_stage[next[cnt_box].y][next[cnt_box].x] == BOX) {
	//			continue;
	//		}
	//		else {
	//			cout << "error" << endl;
	//			cout << target_stage[next[cnt_box].y][next[cnt_box].x] << "," << next[cnt_box].x << "," << next[cnt_box].y << endl;
	//		}
	//	}
	//	else if (target_stage[current_pos[cnt_box].y][current_pos[cnt_box].x] == BOX_ON_GOAL) {
	//		continue;
	//	}
	//}

	//listなし
	if ((head == NULL) && (tail == NULL))
	{
		return 0;
	}

	while (true)
	{
		//初期化
		flag_check = 1;
		for (int y = 0; y < HEIGHT; y++) {
			for (int x = 0; x < WIDTH; x++) {
					check_list[y][x] = 0;
			}
		}
		//listと比較
		for (int y = 0; y < HEIGHT; y++) {
			for (int x = 0; x < WIDTH; x++) {
				if (nowStageList->stage[y][x] == target_stage[y][x]) {
					check_list[y][x] = 1;
				}
			}
		}
		for (int y = 0; y < HEIGHT; y++) {
			for (int x = 0; x < WIDTH; x++) {
				cout << check_list[y][x];
			}
			cout << endl;
		}
		cout << endl;

		//比較結果を確認
		for (int y = 0; y < HEIGHT; y++) {
			for (int x = 0; x < WIDTH; x++) {
				if (check_list[y][x] == 0) {
					flag_check = 0;
					break;
				}
			}
			if (flag_check == 0) {
				break;
			}
		}
		//履歴にあった場合「１」を返す
		if (flag_check == 1) {
			return 1;
		}

		//listをつぎへ
		if (nowStageList->next_stage == NULL)
			break;
		else
			nowStageList = nowStageList->next_stage;
	}
	return 0;
}