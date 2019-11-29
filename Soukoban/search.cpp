#include "search.h"
#include <iostream>
#include <vector>
#include <queue>
#include <iomanip>
#include <string> 
#include "define.h"
#include "count.h"
#include "print.h"
#include "check.h"

/* 4 �����ւ̗אڒ��_�ւ̈ړ���\���x�N�g�� */
const int dx[4] = { 1, 0, -1, 0 };//�E�A���A���A��
const int dy[4] = { 0, 1, 0, -1 };//�E�A���A���A��

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

SQUARE searchGoal(int stage[][WIDTH]) {
	SQUARE point_goal;
	point_goal.x = 0;
	point_goal.y = 0;
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			if (stage[y][x] == GOAL) {
				point_goal.x = x;
				point_goal.y = y;
				return point_goal;
			}
		}
	}

	return point_goal;
}

int searchBreadthFirst(int stage[][WIDTH]) {
	int search_stage[HEIGHT][WIDTH] = {};//�T���p�̃X�e�[�W	
	SQUARE start_pos;//�ו��̏����ʒu
	SQUARE goal_pos;//�ו��̏����ʒu
	SQUARE movable_pos;//���[�p
	SQUARE current_pos;//�ו��̌��ݍ��W
	std::queue<SQUARE> search;//�ړ��\�ł��薢���B�̍��W��ۑ��i�T���p�L���[�j
	std::vector<std::vector<int>> dist(HEIGHT, std::vector<int>(WIDTH,-1));//�ʉ߂������W�̃X�^�[�g����̋���
	// �T�����Ɋe�}�X�͂ǂ̃}�X���痈���̂���\���z�� (�ŒZ�o�H����m�邾���Ȃ�A����͕s�v)
	std::vector<std::vector<int> > prev_x(HEIGHT, std::vector<int>(WIDTH, -1));
	std::vector<std::vector<int> > prev_y(HEIGHT, std::vector<int>(WIDTH, -1));
	                    /*(�c�̃T�C�Y,(���̃T�C�Y,����������l))*/

	//
	/*�����ݒ�*/
	//
	/*�T���p�X�e�[�W�̏�����*/
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			search_stage[y][x] = stage[y][x];
		}
	}
	/*�����ʒu���Z�b�g*/
	start_pos = searchBox(stage);
	goal_pos = searchGoal(stage);
	/*�X�^�[�g�̋������u0�v�ɐݒ�*/
	dist[start_pos.y][start_pos.x] = 0;
	/*�X�^�[�g���v�b�V��*/
	search.push(start_pos);
	current_pos = search.front();


	//
	/*�T��*/
	//
	while (!search.empty())
	{
		/*�ړ��i�Ֆʂ̕ύX�j*/
		search_stage[current_pos.y][current_pos.x] = PATH;
		if (search_stage[search.front().y][search.front().x] == PATH) {
			search_stage[search.front().y][search.front().x] = BOX;
		}
		else if (search_stage[search.front().y][search.front().x] == GOAL) {
			search_stage[search.front().y][search.front().x] = BOX_ON_GOAL;
		}
		current_pos = search.front();
		search.pop();

		/*���͂̈ړ��\�ȍ��W���L���[�Ɏ��[*/
		for (int direction = 0; direction < 4; direction++) {
			int next_x = current_pos.x + dx[direction];
			int next_y = current_pos.y + dy[direction];
			// ��O���m
			if (next_x < 0 || next_x >= WIDTH || next_y < 0 || next_y >= HEIGHT) continue; 
			//�ו����ړ��\��
			if (search_stage[next_y][next_x] != WALL && search_stage[next_y][next_x] != BOX && search_stage[next_y][next_x] != MAN) {
				movable_pos.x = next_x;
				movable_pos.y = next_y;
				//�ʉߍς݂��m�F
				if (dist[movable_pos.y][movable_pos.x] == -1) {
					search.push(movable_pos);
					dist[movable_pos.y][movable_pos.x] = dist[current_pos.y][current_pos.x] + 1;
					prev_x[movable_pos.y][movable_pos.x] = current_pos.x;  // �ǂ̒��_�����񂪓`�d���ė������A�c�����̍��W������
					prev_y[movable_pos.y][movable_pos.x] = current_pos.y;  // �ǂ̒��_�����񂪓`�d���ė������A�������̍��W������
				}
			}
		}

		/*�T���̏I���m�F*/
		if (checkClear(search_stage)) {
			std::cout << "Clear" << std::endl;
			printStage(search_stage);
			break;
		}
		if (search.empty()) {
			std::cout << "Not found route" << std::endl;
			return 0;
		}
	}
	
	//
	/* ���ʏo�� */
	//
	/* �e�}�X�ւ̃X�^�[�g����̍ŒZ���������Ă݂� */
	for (int y = 0; y < HEIGHT; ++y) {
		for (int x = 0; x < WIDTH; ++x) {
			if (stage[y][x] == WALL) std::cout << "###";
			else std::cout << std::setw(3) << dist[y][x];
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;

	/* �S�[���Ɏ���܂ł̍ŒZ�o�H�𕜌����Ă݂� */
	int x = goal_pos.x, y = goal_pos.y;
	while (x != -1 && y != -1) {
		search_stage[y][x] = CHECK; // �ʉ߂������Ƃ�����

		// �O�̒��_�֍s��
		int px = prev_x[y][x];
		int py = prev_y[y][x];
		x = px, y = py;
	}

	search_stage[start_pos.y][start_pos.x] = BOX;
	search_stage[goal_pos.y][goal_pos.x] = GOAL;
	printStage(search_stage);

	return 1;
}