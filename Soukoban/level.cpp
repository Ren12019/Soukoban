#include "level.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "grid.h"

//�R���X�g���N�^
Level::Level()
{
}
//�f�X�g���N�^
Level::~Level()
{
}
//�����_����x���W��Ԃ�
int choiceX() {
	return (rand() % (WIDTH - 2)) + 1;
}
//�����_����y���W��Ԃ�
int choiceY() {
	return (rand() % (HEIGHT - 2)) + 1;
}
//�Ώۃ}�X���ו����^�т��ނ��Ƃ��\���𔻒肷��
bool checkCarryInSquare(const int stage[][WIDTH], const int x, const int y) {
	if ((stage[y][x - 1] == PATH || stage[y][x - 1] == GOAL) && (stage[y][x + 1] == PATH || stage[y][x + 1] == GOAL)) {
		if (stage[y][x - 2] == WALL && stage[y][x + 2] == WALL && stage[y - 1][x] == WALL && stage[y + 1][x] == WALL) {
			return true;
		}
	}

	if ((stage[y - 1][x] == PATH || stage[y - 1][x] == GOAL) && (stage[y + 1][x] == PATH || stage[y + 1][x] == GOAL)) {
		if (stage[y - 2][x] == WALL && stage[y + 2][x] == WALL && stage[y][x - 1] == WALL && stage[y][x + 1] == WALL) {
			return true;
		}
	}

	return false;
}
//�`�F�b�N���X�g�ɉו����^�э��߂Ȃ��G���A���`�F�b�N����
void checkCarryInArea(const int stage[][WIDTH], int checklist[][WIDTH]) {
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
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
//���͂̕ǂ̐��𐔂���
int countAroundWall(const int stage[][WIDTH], const int x, const int y) {
	int count = 0;

	if (stage[y - 1][x] == WALL)
		count++;
	if (stage[y + 1][x] == WALL)
		count++;
	if (stage[y][x - 1] == WALL)
		count++;
	if (stage[y][x + 1] == WALL)
		count++;

	return count;
}
//�`�F�b�N���X�g�Ƀ`�F�b�N�����݂��邩���肷��
bool checkChecklist(const int checklist[][WIDTH]) {
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			if (checklist[y][x] != CHECK) {
				return false;
			}
		}
	}
	return true;
}
//�Ώۃ}�X���ו����^�т��ނ��Ƃ��\���𔻒肷��
bool checkSquare(int stage[][WIDTH])
{
	bool flag = false;

	for (int y = 0; y < HEIGHT; y++)
	{
		for (int x = 0; x < WIDTH; x++)
		{
			if (stage[y][x] == PATH)
			{
				if (countAroundWall(stage, x, y) >= 3)
				{
					stage[y][x] = WALL;
					flag = true;
				}
			}
		}
	}

	return flag;
}
//�Ώۃ}�X���ӂɐڂ��Ă��邩���肷��
bool checkNeighborhoodWall(const int stage[][WIDTH], const int x, const int y) {
	//Left
	if (stage[y - 1][x - 1] == WALL && stage[y][x - 1] == WALL && stage[y + 1][x - 1] == WALL) {
		return true;
	}
	//Right
	if (stage[y - 1][x + 1] == WALL && stage[y][x + 1] == WALL && stage[y + 1][x + 1] == WALL) {
		return true;
	}
	//Up
	if (stage[y - 1][x - 1] == WALL && stage[y - 1][x] == WALL && stage[y - 1][x + 1] == WALL) {
		return true;
	}
	//Down
	if (stage[y + 1][x - 1] == WALL && stage[y + 1][x] == WALL && stage[y + 1][x + 1] == WALL) {
		return true;
	}

	return false;
}
//�Ώۃ}�X���p�ł��邩���肷��
bool checkCornerSquare(const int stage[][WIDTH], const int x, const int y) {
	//��
	if (stage[y][x - 1] == WALL && stage[y - 1][x + 1] == WALL && stage[y + 1][x - 1] == WALL && stage[y + 1][x] == WALL) {
		return true;
	}
	//��
	if (stage[y - 1][x - 1] == WALL && stage[y][x + 1] == WALL && stage[y + 1][x] == WALL && stage[y + 1][x + 1] == WALL) {
		return true;
	}
	//��
	if (stage[y - 1][x - 1] == WALL && stage[y - 1][x] == WALL && stage[y][x - 1] == WALL && stage[y + 1][x + 1] == WALL) {
		return true;
	}
	//��
	if (stage[y - 1][x] == WALL && stage[y - 1][x + 1] == WALL && stage[y][x + 1] == WALL && stage[y + 1][x - 1] == WALL) {
		return true;
	}

	return false;
}
//�Ώۃ}�X�ɉו���z�u�ł��邩���肷��
void checkPutBox(const int stage[][WIDTH], int checklist[][WIDTH]) {
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
//�����ɉו���u�����Ƃŋl�݂��������邩���肷��
bool checkDeadlock(const int stage[][WIDTH], const int x, const int y) {
	/*four boxes*/
	if ((stage[y][x + 1] == BOX || stage[y][x + 1] == BOX_ON_GOAL) && (stage[y + 1][x] == BOX || stage[y + 1][x] == BOX_ON_GOAL) && (stage[y + 1][x + 1] == BOX || stage[y + 1][x + 1] == BOX_ON_GOAL)) {
		return true;
	}
	else if ((stage[y][x - 1] == BOX || stage[y][x - 1] == BOX_ON_GOAL) && (stage[y + 1][x] == BOX || stage[y + 1][x] == BOX_ON_GOAL) && (stage[y + 1][x - 1] == BOX || stage[y + 1][x - 1] == BOX_ON_GOAL)) {
		return true;
	}
	else if ((stage[y][x + 1] == BOX || stage[y][x + 1] == BOX_ON_GOAL) && stage[y - 1][x] == BOX && (stage[y - 1][x + 1] == BOX || stage[y - 1][x + 1] == BOX_ON_GOAL)) {
		return true;
	}
	else if ((stage[y][x - 1] == BOX || stage[y][x - 1] == BOX_ON_GOAL) && stage[y - 1][x] == BOX && (stage[y - 1][x - 1] == BOX || stage[y - 1][x - 1] == BOX_ON_GOAL)) {
		return true;
	}
	/*one box two wall*/
	else if (stage[y][x + 1] == WALL && stage[y - 1][x] == WALL) {
		return true;
	}
	else if (stage[y][x - 1] == WALL && stage[y - 1][x] == WALL) {
		return true;
	}
	else if (stage[y][x - 1] == WALL && stage[y + 1][x] == WALL) {
		return true;
	}
	else if (stage[y][x + 1] == WALL && stage[y + 1][x] == WALL) {
		return true;
	}
	/*two box two wall(��)*/
	else if ((stage[y][x + 1] == BOX || stage[y][x + 1] == BOX_ON_GOAL) && stage[y - 1][x] == WALL && stage[y - 1][x + 1] == WALL) {
		return true;
	}
	else if ((stage[y][x - 1] == BOX || stage[y][x - 1] == BOX_ON_GOAL) && stage[y - 1][x] == WALL && stage[y - 1][x - 1] == WALL) {
		return true;
	}
	else if ((stage[y][x + 1] == BOX || stage[y][x + 1] == BOX_ON_GOAL) && stage[y + 1][x] == WALL && stage[y + 1][x + 1] == WALL) {
		return true;
	}
	else if ((stage[y][x - 1] == BOX || stage[y][x - 1] == BOX_ON_GOAL) && stage[y + 1][x] == WALL && stage[y + 1][x - 1] == WALL) {
		return true;
	}
	//
	else if (stage[y - 1][x] == BOX && stage[y][x - 1] == WALL && stage[y - 1][x - 1] == WALL) {
		return true;
	}
	else if ((stage[y + 1][x] == BOX || stage[y + 1][x] == BOX_ON_GOAL) && stage[y][x - 1] == WALL && stage[y + 1][x - 1] == WALL) {
		return true;
	}
	else if (stage[y - 1][x] == BOX && stage[y][x + 1] == WALL && stage[y - 1][x + 1] == WALL) {
		return true;
	}
	else if ((stage[y + 1][x] == BOX || stage[y + 1][x] == BOX_ON_GOAL) && stage[y][x + 1] == WALL && stage[y + 1][x + 1] == WALL) {
		return true;
	}
	/*two box two wall(S)*/
	else if ((stage[y][x + 1] == BOX || stage[y][x + 1] == BOX_ON_GOAL) && stage[y - 1][x] == WALL && stage[y + 1][x + 1] == WALL) {
		return true;
	}
	else if ((stage[y][x + 1] == BOX || stage[y][x + 1] == BOX_ON_GOAL) && stage[y + 1][x] == WALL && stage[y - 1][x + 1] == WALL) {
		return true;
	}
	else if ((stage[y][x - 1] == BOX || stage[y][x - 1] == BOX_ON_GOAL) && stage[y - 1][x] == WALL && stage[y + 1][x - 1] == WALL) {
		return true;
	}
	else if ((stage[y][x - 1] == BOX || stage[y][x - 1] == BOX_ON_GOAL) && stage[y + 1][x] == WALL && stage[y - 1][x - 1] == WALL) {
		return true;
	}
	//
	else if ((stage[y + 1][x] == BOX || stage[y + 1][x] == BOX_ON_GOAL) && stage[y][x + 1] == WALL && stage[y + 1][x - 1] == WALL) {
		return 1;
	}
	else if ((stage[y + 1][x] == BOX || stage[y + 1][x] == BOX_ON_GOAL) && stage[y][x - 1] == WALL && stage[y + 1][x + 1] == WALL) {
		return true;
	}
	else if ((stage[y - 1][x] == BOX || stage[y - 1][x] == BOX_ON_GOAL) && stage[y - 1][x + 1] == WALL && stage[y][x - 1] == WALL) {
		return true;
	}
	else if ((stage[y - 1][x] == BOX || stage[y - 1][x] == BOX_ON_GOAL) && stage[y - 1][x - 1] == WALL && stage[y][x + 1] == WALL) {
		return true;
	}
	/*three boxes one wall*/
	else if (stage[y][x + 1] == WALL && (stage[y + 1][x] == BOX || stage[y + 1][x] == BOX_ON_GOAL) && (stage[y + 1][x + 1] == BOX || stage[y + 1][x + 1] == BOX_ON_GOAL)) {
		return true;
	}
	else if ((stage[y][x + 1] == BOX || stage[y][x + 1] == BOX_ON_GOAL) && stage[y + 1][x] == WALL && (stage[y + 1][x + 1] == BOX || stage[y + 1][x + 1] == BOX_ON_GOAL)) {
		return true;
	}
	else if ((stage[y][x + 1] == BOX || stage[y][x + 1] == BOX_ON_GOAL) && (stage[y + 1][x] == BOX || stage[y + 1][x] == BOX_ON_GOAL) && stage[y + 1][x + 1] == WALL) {
		return true;
	}
	else if (stage[y][x - 1] == WALL && (stage[y + 1][x - 1] == BOX || stage[y + 1][x - 1] == BOX_ON_GOAL) && stage[y + 1][x] == WALL) {
		return true;
	}
	/*three boxes two wall*/
	//1
	else if ((stage[y][x + 1] == BOX || stage[y][x + 1] == BOX_ON_GOAL) && (stage[y + 1][x + 1] == BOX || stage[y + 1][x + 1] == BOX_ON_GOAL) && stage[y - 1][x] == WALL && stage[y + 1][x + 2] == WALL) {
		return true;
	}
	else if ((stage[y][x - 1] == BOX || stage[y][x - 1] == BOX_ON_GOAL) && (stage[y + 1][x] == BOX || stage[y + 1][x] == BOX_ON_GOAL) && stage[y - 1][x - 1] == WALL && stage[y + 1][x + 1] == WALL) {
		return true;
	}
	else if ((stage[y + 1][x] == BOX || stage[y + 1][x] == BOX_ON_GOAL) && (stage[y - 1][x - 1] == BOX || stage[y - 1][x - 1] == BOX_ON_GOAL) && stage[y][x + 1] == WALL && stage[y - 2][x - 1] == WALL) {
		return true;
	}
	//2
	else if ((stage[y][x + 1] == BOX || stage[y][x + 1] == BOX_ON_GOAL) && (stage[y + 1][x] == BOX || stage[y + 1][x] == BOX_ON_GOAL) && stage[y - 1][x + 1] == WALL && stage[y + 1][x - 1] == WALL) {
		return true;
	}
	else if ((stage[y][x - 1] == BOX || stage[y][x - 1] == BOX_ON_GOAL) && (stage[y + 1][x - 1] == BOX || stage[y + 1][x - 1] == BOX_ON_GOAL) && stage[y - 1][x] == WALL && stage[y + 1][x - 2] == WALL) {
		return true;
	}
	else if (stage[y - 1][x] == BOX && (stage[y - 1][x + 1] == BOX || stage[y - 1][x + 1] == BOX_ON_GOAL) && stage[y - 2][x + 1] == WALL && stage[y][x - 1] == WALL) {
		return true;
	}
	//3
	else if ((stage[y][x + 1] == BOX || stage[y][x + 1] == BOX_ON_GOAL) && (stage[y - 1][x + 1] == BOX || stage[y - 1][x + 1] == BOX_ON_GOAL) && stage[y - 1][x + 2] == WALL && stage[y + 1][x] == WALL) {
		return true;
	}
	else if ((stage[y + 1][x] == BOX || stage[y + 1][x] == BOX_ON_GOAL) && (stage[y + 1][x - 1] == BOX || stage[y + 1][x - 1] == BOX_ON_GOAL) && stage[y][x + 1] == WALL && stage[y + 2][x - 1] == WALL) {
		return true;
	}
	else if ((stage[y][x - 1] == BOX || stage[y][x - 1] == BOX_ON_GOAL) && stage[y - 1][x] == BOX && stage[y - 1][x + 1] == WALL && stage[y + 1][x - 1] == WALL) {
		return true;
	}
	//4
	else if ((stage[y + 1][x] == BOX || stage[y + 1][x] == BOX_ON_GOAL) && (stage[y + 1][x + 1] == BOX || stage[y + 1][x + 1] == BOX_ON_GOAL) && stage[y][x - 1] == WALL && stage[y + 2][x + 1] == WALL) {
		return true;
	}
	else if (stage[y - 1][x] == BOX && (stage[y][x + 1] == BOX || stage[y][x + 1] == BOX_ON_GOAL) && stage[y - 1][x - 1] == WALL && stage[y + 1][x + 1] == WALL) {
		return true;
	}
	else if ((stage[y][x - 1] == BOX || stage[y][x - 1] == BOX_ON_GOAL) && (stage[y - 1][x - 1] == BOX || stage[y - 1][x - 1] == BOX_ON_GOAL) && stage[y + 1][x] == WALL && stage[y - 1][x - 2] == WALL) {
		return true;
	}
	return false;
}

//�X�e�[�W�������s��
void Level::createLevel() {
	while (true)
	{
		do {
			//�e���v���[�g�ŋ󕔉��𐶐�
			createEmptyRoom();
			//�Ӗ��̂Ȃ��}�X�𖄂ߐ��n
			fillBlindAlley();
		} while (checkSection() == 1 && countSpace() > 4);

		//�S�[���A�ו��A�v���C���[��z�u
		if (!setGoal()) {
			continue;
		}
		if (!setBox()) {
			continue;
		}
		if (!setPlayer()) {
			continue;
		}

		break;
	}
}
//��̕��������
void Level::createEmptyRoom() {
	Grid grid;

	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			stage[y][x] = WALL;
		}
	}

	for (int grid_num = 0, v = 0, h = 0; grid_num < (NUMBER_OF_GRID_VERTICAL * NUMBER_OF_GRID_HORIZONTAL); grid_num++) {
		v = grid_num / NUMBER_OF_GRID_HORIZONTAL;
		h = grid_num % NUMBER_OF_GRID_HORIZONTAL;
		grid.setGridTemplate();

		for (int y = 0; y < GRID_SIZE; y++) {
			for (int x = 0; x < GRID_SIZE; x++) {
				stage[y + (v * GRID_SIZE) + 1][x + (h * GRID_SIZE) + 1] = grid.grid[y][x];
			}
		}
	}
}
//�܏��H�ȂǈӖ��̂Ȃ��X�y�[�X�𖄂߂�
void Level::fillBlindAlley() {
	while (checkSquare(stage) != 0) {}
}
//�S�[����ݒu����
bool Level::setGoal() {
	int x = 0, y = 0;
	int checklist[HEIGHT][WIDTH] = {};

	checkCarryInArea(stage, checklist);

	for (int num_box = NUMBER_OF_BOX; num_box != 0; num_box--) {
		while (true) {
			x = choiceX();
			y = choiceY();
			if (stage[y][x] == PATH && checklist[y][x] != CHECK) {
				stage[y][x] = GOAL;
				break;
			}
		}
	}
	return true;
}
//�ו���ݒu����
bool Level::setBox() {
	clock_t t = clock();
	int x = 0, y = 0;
	int checklist[HEIGHT][WIDTH] = {};

	for (int num_box = NUMBER_OF_BOX; num_box != 0; num_box--) {
		checkPutBox(stage, checklist);

		while (true) {
			x = choiceX();
			y = choiceY();
			if ((stage[y][x] == PATH || stage[y][x] == GOAL) && checklist[y][x] != CHECK) {
				if (checkDeadlock(stage, x, y)) {
					checklist[y][x] = CHECK;
				}
				else {
					if (stage[y][x] == PATH) {
						stage[y][x] = BOX;
						if (checkSection())
						{
							stage[y][x] = PATH;
						}
						else {
							break;
						}
					}
					else if (stage[y][x] == GOAL) {
						stage[y][x] = BOX_ON_GOAL;
						if (checkSection())
						{
							stage[y][x] = GOAL;
						}
						else {
							break;
						}
					}
				}
			}

			if (checkChecklist(checklist)) {
				printf("Failed to create stage:Not Find Space\n");
				return false;
			}
			if (clock() - t > 2000) {
				printf("Failed to create stage:Time Out\n");
				return false;
			}
		}
	}

	return true;

}
//�v���C���[��z�u����
bool Level::setPlayer() {
	int x = 0, y = 0;

	while (true) {
		x = choiceX();
		y = choiceY();
		if (stage[y][x] == PATH) {
			stage[y][x] = MAN;
			return true;
		}
		else if (stage[y][x] == GOAL) {
			stage[y][x] = MAN_ON_GOAL;
			return true;
		}
	}
}
//�X�e�[�W���R�}���h�v�����v�g�֏o�͂���
void Level::printStage() {
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			if (stage[y][x] == WALL)
				printf("#");
			else if (stage[y][x] == MAN)
				printf("@");
			else if (stage[y][x] == MAN_ON_GOAL)
				printf("+");
			else if (stage[y][x] == BOX)
				printf("$");
			else if (stage[y][x] == BOX_ON_GOAL)
				printf("*");
			else if (stage[y][x] == GOAL)
				printf(".");
			else if (stage[y][x] == PATH)
				printf(" ");
			else if (stage[y][x] == CHECK)
				printf("x");
		}
		printf("\n");
	}
	printf("\n");
}
//�X�e�[�W���e�L�X�g�t�@�C���ɏo�͂���
void Level::outputStage() {
	FILE *fp;

	if (fopen_s(&fp, "StageData/stage.txt", "w") != 0) {
		printf("�t�@�C�����J���܂���ł����B\n");
		return;
	}
	//�������o��
	fprintf(fp, "%d\n", WIDTH);

	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			if (stage[y][x] == WALL)
				fprintf(fp, "#");
			else if (stage[y][x] == MAN)
				fprintf(fp, "@");
			else if (stage[y][x] == MAN_ON_GOAL)
				fprintf(fp, "+");
			else if (stage[y][x] == BOX)
				fprintf(fp, "$");
			else if (stage[y][x] == BOX_ON_GOAL)
				fprintf(fp, "*");
			else if (stage[y][x] == GOAL)
				fprintf(fp, ".");
			else if (stage[y][x] == PATH)
				fprintf(fp, " ");
		}
		fprintf(fp, "\n");
	}

	fclose(fp);
}
//�r�؂ꂽ�X�y�[�X���ł��Ă����true��Ԃ�
bool Level::checkSection() {
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
				return true;
			}
		}
	}

	return false;
}
//�X�e�[�W�̃}�X���J�E���g���Ԃ�
int Level::countSpace() {
	int count = 0;

	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			if (stage[y][x] != WALL) {
				count++;
			}
		}
	}

	return count;
}
