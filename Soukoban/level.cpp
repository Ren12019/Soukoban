#include "level.h"
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include "grid.h"

//�R���X�g���N�^
Level::Level()
{
	std::vector<char> temp(WIDTH,'#');

	for (int y = 0; y < HEIGHT; y++) {
		stage.emplace_back(temp);
	}
}
//�f�X�g���N�^
Level::~Level()
{
}
//�Ώۃ}�X���ו����^�т��ނ��Ƃ��\���𔻒肷��
bool Level::checkCarryInSquare(const int x, const int y) {
	if ((stage[y][x - 1] == ' ' || stage[y][x - 1] == '.') && (stage[y][x + 1] == ' ' || stage[y][x + 1] == '.')) {
		if (stage[y][x - 2] == '#' && stage[y][x + 2] == '#' && stage[y - 1][x] == '#' && stage[y + 1][x] == '#') {
			return true;
		}
	}

	if ((stage[y - 1][x] == ' ' || stage[y - 1][x] == '.') && (stage[y + 1][x] == ' ' || stage[y + 1][x] == '.')) {
		if (stage[y - 2][x] == '#' && stage[y + 2][x] == '#' && stage[y][x - 1] == '#' && stage[y][x + 1] == '#') {
			return true;
		}
	}

	return false;
}
// �`�F�b�N���X�g�ɉו����^�э��߂Ȃ��G���A���`�F�b�N����
std::vector<SQUARE> Level::checkCarryInArea() {
	std::vector<SQUARE>checklist;
	SQUARE square;
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			if (stage[y][x] == ' ') {
				if (!checkCarryInSquare(x, y)) {
					square.x = x;
					square.y = y;
					checklist.push_back(square);
				}
			}
		}
	}

	return checklist;
}
//�S�[�����u����ꏊ���L���[����
std::vector<SQUARE> Level::decisionCanditdate() {
	std::vector<SQUARE>canditdate;
	SQUARE square;
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			if (stage[y][x] == ' ') {
				if (!checkCarryInSquare(x, y)) {
					square.x = x;
					square.y = y;
					canditdate.push_back(square);
				}
			}
		}
	}

	return canditdate;
}
//���͂̕ǂ̐��𐔂���
int Level::countAroundWall(const int x, const int y) {
	int count = 0;

	if (stage[y - 1][x] == '#')
		count++;
	if (stage[y + 1][x] == '#')
		count++;
	if (stage[y][x - 1] == '#')
		count++;
	if (stage[y][x + 1] == '#')
		count++;

	return count;
}
//�Ώۃ}�X���ו����^�т��ނ��Ƃ��\���𔻒肷��
bool Level::checkSquare()
{
	bool flag = false;

	for (int y = 0; y < HEIGHT; y++)
	{
		for (int x = 0; x < WIDTH; x++)
		{
			if (stage[y][x] == ' ')
			{
				if (countAroundWall(x, y) >= 3)
				{
					stage[y][x] = '#';
					flag = true;
				}
			}
		}
	}

	return flag;
}
//�Ώۃ}�X���p�ł��邩���肷��
bool Level::checkCornerSquare(const int x, const int y){
	//��
	if (stage[y][x - 1] == '#' && stage[y + 1][x] == '#') {
		return true;
	}
	//��
	if (stage[y][x + 1] == '#' && stage[y + 1][x] == '#') {
		return true;
	}
	//��
	if (stage[y - 1][x] == '#' && stage[y][x - 1] == '#') {
		return true;
	}
	//��
	if (stage[y - 1][x] == '#' && stage[y][x + 1] == '#') {
		return true;
	}

	return false;
}
//�ו���z�u�ł�����W�����X�g�����Ԃ�
std::vector<SQUARE> Level::checkPutBox() {
	std::vector<SQUARE>checklist;
	SQUARE square;

	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			if (stage[y][x] == ' ') {
				//�z�u�\��ꏊ���p�ŏo�Ȃ���
				if (!checkCornerSquare(x, y)) {
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
bool Level::checkDeadlock(const int x, const int y) {
	/*four boxes*/
	if ((stage[y][x + 1] == '$' || stage[y][x + 1] == '*') && (stage[y + 1][x] == '$' || stage[y + 1][x] == '*') && (stage[y + 1][x + 1] == '$' || stage[y + 1][x + 1] == '*')) {
		return true;
	}
	else if ((stage[y][x - 1] == '$' || stage[y][x - 1] == '*') && (stage[y + 1][x] == '$' || stage[y + 1][x] == '*') && (stage[y + 1][x - 1] == '$' || stage[y + 1][x - 1] == '*')) {
		return true;
	}
	else if ((stage[y][x + 1] == '$' || stage[y][x + 1] == '*') && stage[y - 1][x] == '$' && (stage[y - 1][x + 1] == '$' || stage[y - 1][x + 1] == '*')) {
		return true;
	}
	else if ((stage[y][x - 1] == '$' || stage[y][x - 1] == '*') && stage[y - 1][x] == '$' && (stage[y - 1][x - 1] == '$' || stage[y - 1][x - 1] == '*')) {
		return true;
	}
	/*one box two wall*/
	else if (stage[y][x + 1] == '#' && stage[y - 1][x] == '#') {
		return true;
	}
	else if (stage[y][x - 1] == '#' && stage[y - 1][x] == '#') {
		return true;
	}
	else if (stage[y][x - 1] == '#' && stage[y + 1][x] == '#') {
		return true;
	}
	else if (stage[y][x + 1] == '#' && stage[y + 1][x] == '#') {
		return true;
	}
	/*two box two wall(��)*/
	else if ((stage[y][x + 1] == '$' || stage[y][x + 1] == '*') && stage[y - 1][x] == '#' && stage[y - 1][x + 1] == '#') {
		return true;
	}
	else if ((stage[y][x - 1] == '$' || stage[y][x - 1] == '*') && stage[y - 1][x] == '#' && stage[y - 1][x - 1] == '#') {
		return true;
	}
	else if ((stage[y][x + 1] == '$' || stage[y][x + 1] == '*') && stage[y + 1][x] == '#' && stage[y + 1][x + 1] == '#') {
		return true;
	}
	else if ((stage[y][x - 1] == '$' || stage[y][x - 1] == '*') && stage[y + 1][x] == '#' && stage[y + 1][x - 1] == '#') {
		return true;
	}
	//
	else if (stage[y - 1][x] == '$' && stage[y][x - 1] == '#' && stage[y - 1][x - 1] == '#') {
		return true;
	}
	else if ((stage[y + 1][x] == '$' || stage[y + 1][x] == '*') && stage[y][x - 1] == '#' && stage[y + 1][x - 1] == '#') {
		return true;
	}
	else if (stage[y - 1][x] == '$' && stage[y][x + 1] == '#' && stage[y - 1][x + 1] == '#') {
		return true;
	}
	else if ((stage[y + 1][x] == '$' || stage[y + 1][x] == '*') && stage[y][x + 1] == '#' && stage[y + 1][x + 1] == '#') {
		return true;
	}
	/*two box two wall(S)*/
	else if ((stage[y][x + 1] == '$' || stage[y][x + 1] == '*') && stage[y - 1][x] == '#' && stage[y + 1][x + 1] == '#') {
		return true;
	}
	else if ((stage[y][x + 1] == '$' || stage[y][x + 1] == '*') && stage[y + 1][x] == '#' && stage[y - 1][x + 1] == '#') {
		return true;
	}
	else if ((stage[y][x - 1] == '$' || stage[y][x - 1] == '*') && stage[y - 1][x] == '#' && stage[y + 1][x - 1] == '#') {
		return true;
	}
	else if ((stage[y][x - 1] == '$' || stage[y][x - 1] == '*') && stage[y + 1][x] == '#' && stage[y - 1][x - 1] == '#') {
		return true;
	}
	//
	else if ((stage[y + 1][x] == '$' || stage[y + 1][x] == '*') && stage[y][x + 1] == '#' && stage[y + 1][x - 1] == '#') {
		return 1;
	}
	else if ((stage[y + 1][x] == '$' || stage[y + 1][x] == '*') && stage[y][x - 1] == '#' && stage[y + 1][x + 1] == '#') {
		return true;
	}
	else if ((stage[y - 1][x] == '$' || stage[y - 1][x] == '*') && stage[y - 1][x + 1] == '#' && stage[y][x - 1] == '#') {
		return true;
	}
	else if ((stage[y - 1][x] == '$' || stage[y - 1][x] == '*') && stage[y - 1][x - 1] == '#' && stage[y][x + 1] == '#') {
		return true;
	}
	/*three boxes one wall*/
	else if (stage[y][x + 1] == '#' && (stage[y + 1][x] == '$' || stage[y + 1][x] == '*') && (stage[y + 1][x + 1] == '$' || stage[y + 1][x + 1] == '*')) {
		return true;
	}
	else if ((stage[y][x + 1] == '$' || stage[y][x + 1] == '*') && stage[y + 1][x] == '#' && (stage[y + 1][x + 1] == '$' || stage[y + 1][x + 1] == '*')) {
		return true;
	}
	else if ((stage[y][x + 1] == '$' || stage[y][x + 1] == '*') && (stage[y + 1][x] == '$' || stage[y + 1][x] == '*') && stage[y + 1][x + 1] == '#') {
		return true;
	}
	else if (stage[y][x - 1] == '#' && (stage[y + 1][x - 1] == '$' || stage[y + 1][x - 1] == '*') && stage[y + 1][x] == '#') {
		return true;
	}
	/*three boxes two wall*/
	//1
	else if ((stage[y][x + 1] == '$' || stage[y][x + 1] == '*') && (stage[y + 1][x + 1] == '$' || stage[y + 1][x + 1] == '*') && stage[y - 1][x] == '#' && stage[y + 1][x + 2] == '#') {
		return true;
	}
	else if ((stage[y][x - 1] == '$' || stage[y][x - 1] == '*') && (stage[y + 1][x] == '$' || stage[y + 1][x] == '*') && stage[y - 1][x - 1] == '#' && stage[y + 1][x + 1] == '#') {
		return true;
	}
	else if ((stage[y + 1][x] == '$' || stage[y + 1][x] == '*') && (stage[y - 1][x - 1] == '$' || stage[y - 1][x - 1] == '*') && stage[y][x + 1] == '#' && stage[y - 2][x - 1] == '#') {
		return true;
	}
	//2
	else if ((stage[y][x + 1] == '$' || stage[y][x + 1] == '*') && (stage[y + 1][x] == '$' || stage[y + 1][x] == '*') && stage[y - 1][x + 1] == '#' && stage[y + 1][x - 1] == '#') {
		return true;
	}
	else if ((stage[y][x - 1] == '$' || stage[y][x - 1] == '*') && (stage[y + 1][x - 1] == '$' || stage[y + 1][x - 1] == '*') && stage[y - 1][x] == '#' && stage[y + 1][x - 2] == '#') {
		return true;
	}
	else if (stage[y - 1][x] == '$' && (stage[y - 1][x + 1] == '$' || stage[y - 1][x + 1] == '*') && stage[y - 2][x + 1] == '#' && stage[y][x - 1] == '#') {
		return true;
	}
	//3
	else if ((stage[y][x + 1] == '$' || stage[y][x + 1] == '*') && (stage[y - 1][x + 1] == '$' || stage[y - 1][x + 1] == '*') && stage[y - 1][x + 2] == '#' && stage[y + 1][x] == '#') {
		return true;
	}
	else if ((stage[y + 1][x] == '$' || stage[y + 1][x] == '*') && (stage[y + 1][x - 1] == '$' || stage[y + 1][x - 1] == '*') && stage[y][x + 1] == '#' && stage[y + 2][x - 1] == '#') {
		return true;
	}
	else if ((stage[y][x - 1] == '$' || stage[y][x - 1] == '*') && stage[y - 1][x] == '$' && stage[y - 1][x + 1] == '#' && stage[y + 1][x - 1] == '#') {
		return true;
	}
	//4
	else if ((stage[y + 1][x] == '$' || stage[y + 1][x] == '*') && (stage[y + 1][x + 1] == '$' || stage[y + 1][x + 1] == '*') && stage[y][x - 1] == '#' && stage[y + 2][x + 1] == '#') {
		return true;
	}
	else if (stage[y - 1][x] == '$' && (stage[y][x + 1] == '$' || stage[y][x + 1] == '*') && stage[y - 1][x - 1] == '#' && stage[y + 1][x + 1] == '#') {
		return true;
	}
	else if ((stage[y][x - 1] == '$' || stage[y][x - 1] == '*') && (stage[y - 1][x - 1] == '$' || stage[y - 1][x - 1] == '*') && stage[y + 1][x] == '#' && stage[y - 1][x - 2] == '#') {
		return true;
	}
	return false;
}
//�v���C���[��z�u�ł�����W�����X�g�����Ԃ�
std::vector<SQUARE> Level::checkPutPlayer() {
	std::vector<SQUARE>checklist;
	SQUARE square;

	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			if (stage[y][x] == ' ') {
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
	} while (checkSection() == 1 || countSpace() < 8);
}
//�܏��H�ȂǈӖ��̂Ȃ��X�y�[�X�𖄂߂�
void Level::fillBlindAlley() {
	while (checkSquare() != 0) {}
}
//�S�[����ݒu����
bool Level::setGoal() {
	std::vector<SQUARE>checklist;
	//�z�u�\�ȍ��W��vector��
	checklist = checkCarryInArea();
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
		if (stage[y][x] == ' ') 
			stage[y][x] = '.';
	}

	return true;
}
//�S�[����ݒu����
bool Level::setBoxOnGoal() {
	std::vector<SQUARE>checklist;
	//�z�u�\�ȍ��W��vector��
	checklist = checkCarryInArea();
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
#if 1
	for (int num_box = NUMBER_OF_BOX; num_box != 0; num_box--) {
		SQUARE set_square = checklist[rand() % checklist.size()];
		int x = set_square.x;
		int y = set_square.y;
		if (stage[y][x] == ' ')
			stage[y][x] = '*';
	}
#else
#endif
	return true;
}
void Level::setBoxOnGoal(SQUARE set_pos) {
	int x = set_pos.x;
	int y = set_pos.y;
	if (stage[y][x] == ' ')
		stage[y][x] = '*';
}
//�ו���ݒu����
bool Level::setBox() {
	for (int num_box = NUMBER_OF_BOX; num_box != 0; num_box--) {
		//�z�u�\�ȍ��W��vector��
		std::vector<SQUARE> checklist = checkPutBox();
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
		if (!checkDeadlock(x, y)) {
			stage[y][x] = '$';
		}
	}

	return true;
}
//�v���C���[��z�u����
bool Level::setPlayer() {
	//�z�u�\�ȍ��W��vector��
	std::vector<SQUARE> checklist = checkPutPlayer();

	//�z�u�ł���ꏊ�����݂��Ȃ�
	if (checklist.empty()) {
		printf("�ו��̔z�u�Ɏ��s���܂����B\n");
		return false;
	}
	//���X�g�̒����烉���_����
	SQUARE set_square = checklist[rand() % checklist.size()];
	int x = set_square.x;
	int y = set_square.y;
	stage[y][x] = '@';
	
	return true;
}
//�X�e�[�W���R�}���h�v�����v�g�֏o�͂���
void Level::printStage() {
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			if (stage[y][x] == '#')
				printf("#");
			else if (stage[y][x] == '@')
				printf("@");
			else if (stage[y][x] == '+')
				printf("+");
			else if (stage[y][x] == '$')
				printf("$");
			else if (stage[y][x] == '*')
				printf("*");
			else if (stage[y][x] == '.')
				printf(".");
			else if (stage[y][x] == ' ')
				printf(" ");
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
			if (stage[y][x] == '#')
				fprintf(fp, "#");
			else if (stage[y][x] == '@')
				fprintf(fp, "@");
			else if (stage[y][x] == '+')
				fprintf(fp, "+");
			else if (stage[y][x] == '$')
				fprintf(fp, "$");
			else if (stage[y][x] == '*')
				fprintf(fp, "*");
			else if (stage[y][x] == '.')
				fprintf(fp, ".");
			else if (stage[y][x] == ' ')
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
			if (stage[y][x] == ' ' || stage[y][x] == '.')
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
			if (stage[y][x] != '#') {
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
			if (stage[y][x] != '#') {
				stage[y][x] = ' ';
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
			if (stage[y][x] == '#')
				line+="#";
			else if (stage[y][x] == '@')
				line += "@";
			else if (stage[y][x] == '+')
				line += "+";
			else if (stage[y][x] == '$')
				line += "$";
			else if (stage[y][x] == '*')
				line += "*";
			else if (stage[y][x] == '.')
				line += ".";
			else if (stage[y][x] == ' ')
				line += " ";
		}
		line += "\n";
	}

	return line;
}
//�X�e�[�W��string�^�œ���
void Level::inputString(std::string input) {
	std::stringstream ss(input);
	std::string line;
	int counter = 0;
	std::vector< std::vector<char> > copy_stage;
	while (getline(ss, line, '\n'))
	{
		std::vector<char> temp;
		copy_stage.push_back(temp);
		for (unsigned int i = 0; i < line.length(); i++)
		{
			copy_stage[counter].push_back(line[i]);
		}
		counter++;
	}
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			stage[y][x] = copy_stage[y][x];
		}
	}
}