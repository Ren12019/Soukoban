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
bool Level::canCarryInSquare(const SQUARE square) {
	const int x = square.x;
	const int y = square.y;

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
std::vector<SQUARE> Level::storeCarryInArea() {
	std::vector<SQUARE>checklist;
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			if (stage[y][x] == ' ') {
				SQUARE square = { x,y };
				if (!canCarryInSquare(square)) {
					checklist.push_back(square);
				}
			}
		}
	}

	return checklist;
}
//�S�[�����u����ꏊ���L���[����
std::vector<SQUARE> Level::storeCandidate() {
	std::vector<SQUARE>canditdate;
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			if (stage[y][x] == ' ') {
				SQUARE square = { x,y };
				if (!canCarryInSquare(square)) {
					canditdate.push_back(square);
				}
			}
		}
	}

	return canditdate;
}
//���͂̕ǂ̐��𐔂���
int Level::countAroundWall(const SQUARE square) {
	const int x = square.x;
	const int y = square.y;
	int cnt_wall = 0;

	if (stage[y - 1][x] == '#')
		cnt_wall++;
	if (stage[y + 1][x] == '#')
		cnt_wall++;
	if (stage[y][x - 1] == '#')
		cnt_wall++;
	if (stage[y][x + 1] == '#')
		cnt_wall++;

	return cnt_wall;
}
//�e���v���[�g���g�p���ăO���b�h����󕔉����쐬����
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
//�܏��H�𖄂߂�
void Level::fillBlindAlley() {
	bool exist = true;
	while (exist) {
		exist = false;
		for (int y = 0; y < HEIGHT; y++) {
			for (int x = 0; x < WIDTH; x++) {
				if (stage[y][x] == ' ') {
					SQUARE square = { x,y };
					if (countAroundWall(square) >= 3) {
						stage[y][x] = '#';
						exist = true;
					}
				}
			}
		}
	}
}
//�S�[����ݒu����
bool Level::setBoxOnGoal(SQUARE set_pos) {
	int x = set_pos.x;
	int y = set_pos.y;
	if (stage[y][x] == ' ') {
		stage[y][x] = '*';
		return true;
	}

	return false;
}
//�A�o�^�[��z�u����
bool Level::setPlayer() {
	//�z�u�\�ȍ��W��vector��
	std::vector<SQUARE> candidate = storeCandidate();

	//�z�u�ł���ꏊ�����݂��Ȃ�
	if (candidate.empty()) {
		std::cout << "�A�o�^�[�̔z�u�Ɏ��s���܂����B" << std::endl;
		return false;
	}
	//���X�g�̒����烉���_����
	SQUARE set_square = candidate[rand() % candidate.size()];
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
bool Level::isTwoRoom() {
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
//���ʂȋ�Ԃ̊m�F
bool Level::hasLargeSpace() {
	//bool check_space = false;
	//���ƍ������v��
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			//�X�y�[�X��T��
			if (stage[y][x] == ' ') {
				//3*4
				//�X�y�[�X�݂̂̒����`�ł��邩
				for (int cnt_y = 0; cnt_y < 4; cnt_y++) {
					int h = y + cnt_y;
					if (h >= HEIGHT) {
						break;
					}
					int w;
					for (int cnt_x = 0; cnt_x < 3; cnt_x++) {
						w = x + cnt_x;
						if (w >= WIDTH) {
							break;
						}
						if (stage[h][w] == '#') {
							break;
						}
						else if (w == x + 2 && h == y + 3) {
							return true;
						}
					}
					if (stage[h][w] == '#') {
						break;
					}
				}
				//4*3
				//�v���������ƍ����̋�Ԃ��X�y�[�X�݂̂̒����`�ł��邩
				for (int cnt_y = 0; cnt_y < 3; cnt_y++) {
					int h = y + cnt_y;
					if (h >= HEIGHT) {
						break;
					}
					int w;
					for (int cnt_x = 0; cnt_x < 4; cnt_x++) {
						w = x + cnt_x;
						if (w >= WIDTH) {
							break;
						}
						if (stage[h][w] == '#') {
							break;
						}
						else if (w == x + 3 && h == y + 2) {
							return true;
						}
					}
					if (stage[h][w] == '#') {
						break;
					}
				}
			}
		}
	}
	
	return false;
}
//�����𖞂������󕔉������
void Level::setEmptyRoom() {
	//2�����łȂ��A8�}�X�ȏォ��3*4,4*3�̒����`�̋�Ԃ��Ȃ����̂𐶐�
	do {
		//�e���v���[�g�ŋ󕔉��𐶐�
		createEmptyRoom();
		//�Ӗ��̂Ȃ��}�X�𖄂ߐ��n
		fillBlindAlley();
	} while (isTwoRoom() || countSpace() < 13 || hasLargeSpace());
}