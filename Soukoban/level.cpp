#include "level.h"
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
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
std::vector<SQUARE> checkCarryInArea(const int stage[][WIDTH]) {
	std::vector<SQUARE>checklist;
	SQUARE square;
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			if (stage[y][x] == PATH) {
				if (!checkCarryInSquare(stage, x, y)) {
					square.x = x;
					square.y = y;
					checklist.push_back(square);
				}
			}
		}
	}

	return checklist;
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
	if (stage[y][x - 1] == WALL && stage[y + 1][x] == WALL) {
		return true;
	}
	//��
	if (stage[y][x + 1] == WALL && stage[y + 1][x] == WALL) {
		return true;
	}
	//��
	if (stage[y - 1][x] == WALL && stage[y][x - 1] == WALL) {
		return true;
	}
	//��
	if (stage[y - 1][x] == WALL && stage[y][x + 1] == WALL) {
		return true;
	}

	return false;
}
//�ו���z�u�ł�����W�����X�g�����Ԃ�
std::vector<SQUARE> checkPutBox(const int stage[][WIDTH]) {
	std::vector<SQUARE>checklist;
	SQUARE square;

	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			if (stage[y][x] == PATH) {
				/*if (checkNeighborhoodWall(stage, x, y)) {
					checklist[y][x] = CHECK;
				}
				else */
				//�z�u�\��ꏊ���p�ŏo�Ȃ���
				if (!checkCornerSquare(stage, x, y)) {
					square.x = x;
					square.y = y;
					checklist.push_back(square);
				}
			}
		}
	}

	return checklist;
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
//�v���C���[��z�u�ł�����W�����X�g�����Ԃ�
std::vector<SQUARE> checkPutPlayer(const int stage[][WIDTH]) {
	std::vector<SQUARE>checklist;
	SQUARE square;

	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			if (stage[y][x] == PATH) {
				square.x = x;
				square.y = y;
				checklist.push_back(square);
			}
		}
	}

	return checklist;
}

//�X�e�[�W�������s��
void Level::createLevel() {
	while (true)
	{
		setEmptyRoom();

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
//��̕��������
void Level::setEmptyRoom() {
	do {
		//�e���v���[�g�ŋ󕔉��𐶐�
		createEmptyRoom();
		//�Ӗ��̂Ȃ��}�X�𖄂ߐ��n
		fillBlindAlley();
	} while (checkSection() == 1 && countSpace() > 8);
}
//�܏��H�ȂǈӖ��̂Ȃ��X�y�[�X�𖄂߂�
void Level::fillBlindAlley() {
	while (checkSquare(stage) != 0) {}
}
//�S�[����ݒu����
bool Level::setGoal() {
	std::vector<SQUARE>checklist;
	//�z�u�\�ȍ��W��vector��
	checklist = checkCarryInArea(stage);
	//�z�u�ł���ꏊ�����݂��Ȃ�
	if (checklist.empty()) {
		printf("�S�[���̔z�u�Ɏ��s���܂����B\n");
		return false;
	}
	//�z�u�ł���ꏊ������Ȃ�
	else if (checklist.size() < NUMBER_OF_BOX) {
		printf("�S�[���̔z�u�Ɏ��s���܂����B\n");
		return false;
	}
	//���X�g�̒����烉���_����
	for (int num_box = NUMBER_OF_BOX; num_box != 0; num_box--) {
		SQUARE set_square = checklist[rand() % checklist.size()];
		int x = set_square.x;
		int y = set_square.y;
		if (stage[y][x] == PATH) 
			stage[y][x] = GOAL;
	}

	return true;
}
//�S�[����ݒu����
bool Level::setBoxOnGoal() {
	std::vector<SQUARE>checklist;
	//�z�u�\�ȍ��W��vector��
	checklist = checkCarryInArea(stage);
	//�z�u�ł���ꏊ�����݂��Ȃ�
	if (checklist.empty()) {
		printf("�S�[����̉ו��̔z�u�Ɏ��s���܂����B\n");
		return false;
	}
	//�z�u�ł���ꏊ������Ȃ�
	else if (checklist.size() < NUMBER_OF_BOX) {
		printf("�S�[����̉ו��̔z�u�Ɏ��s���܂����B\n");
		return false;
	}
	//���X�g�̒����烉���_����
	for (int num_box = NUMBER_OF_BOX; num_box != 0; num_box--) {
		SQUARE set_square = checklist[rand() % checklist.size()];
		int x = set_square.x;
		int y = set_square.y;
		if (stage[y][x] == PATH)
			stage[y][x] = BOX_ON_GOAL;
	}

	return true;
}
//�ו���ݒu����
bool Level::setBox() {
	for (int num_box = NUMBER_OF_BOX; num_box != 0; num_box--) {
		//�z�u�\�ȍ��W��vector��
		std::vector<SQUARE> checklist = checkPutBox(stage);
		//�z�u�ł���ꏊ�����݂��Ȃ�
		if (checklist.empty()) {
			printf("�ו��̔z�u�Ɏ��s���܂����B\n");
			return false;
		}
		//�z�u�ł���ꏊ������Ȃ�
		else if (checklist.size() < NUMBER_OF_BOX) {
			printf("�ו��̔z�u�Ɏ��s���܂����B\n");
			return false;
		}
		//���X�g�̒����烉���_����
		SQUARE set_square = checklist[rand() % checklist.size()];
		int x = set_square.x;
		int y = set_square.y;
		//�l�݂����Ȃ���Δz�u
		if (!checkDeadlock(stage, x, y)) {
			stage[y][x] = BOX;
		}
	}

	return true;
}
//�v���C���[��z�u����
bool Level::setPlayer() {
	//�z�u�\�ȍ��W��vector��
	std::vector<SQUARE> checklist = checkPutPlayer(stage);

	//�z�u�ł���ꏊ�����݂��Ȃ�
	if (checklist.empty()) {
		printf("�ו��̔z�u�Ɏ��s���܂����B\n");
		return false;
	}
	//���X�g�̒����烉���_����
	SQUARE set_square = checklist[rand() % checklist.size()];
	int x = set_square.x;
	int y = set_square.y;
	stage[y][x] = MAN;
	
	return true;
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
//��̕����͂��̂܂܂Ŕz�u�������Z�b�g����
void Level::resetStage() {
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			if (stage[y][x] != WALL) {
				stage[y][x] = PATH;
			}
		}
	}
}
//��̕����ɔz�u�������ׂăZ�b�g����
void Level::setStage() {
	while (true)
	{
		//�S�[���A�ו��A�v���C���[��z�u
		if (!setGoal()) {
			resetStage();
			printf("���̌`�ł͔z�u�ł��܂���B\n");
			printStage();
			setEmptyRoom();
			continue;
		}
		if (!setBox()) {
			resetStage();
			continue;
		}
		if (!setPlayer()) {
			resetStage();
			continue;
		}

		break;
	}
}
//�X�e�[�W��string�^�ŏo��
std::string Level::outputString() {
	std::string line = {};
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			if (stage[y][x] == WALL)
				line+="#";
			else if (stage[y][x] == MAN)
				line += "@";
			else if (stage[y][x] == MAN_ON_GOAL)
				line += "+";
			else if (stage[y][x] == BOX)
				line += "$";
			else if (stage[y][x] == BOX_ON_GOAL)
				line += "*";
			else if (stage[y][x] == GOAL)
				line += ".";
			else if (stage[y][x] == PATH)
				line += " ";
		}
		line += "\n";
	}

	return line;
}