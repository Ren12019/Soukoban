#include "search.h"
#include <iostream>
#include <vector>
#include <queue>
#include <iomanip>
#include <string> 
#include "define.h"
#include "count.h"
#include "print.h"

using namespace std; 

typedef struct edge {
	SQUARE to;
	int weight;
	struct edge(SQUARE t, int w) : to(t), weight(w) { };
}EDGE;

using TargetStage = vector<vector<EDGE>>;

/* 4 �����ւ̗אڒ��_�ւ̈ړ���\���x�N�g�� */
const int dx[4] = { 1, 0, -1, 0 };
const int dy[4] = { 0, 1, 0, -1 };

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
			if (stage[y][x] == BOX) {
				point_goal.x = x;
				point_goal.y = y;
				return point_goal;
			}
		}
	}

	return point_goal;
}

//EDGE searchAround(int stage[][WIDTH]) {
//	EDGE a;
//
//	return ;
//}

int searchBreadthFirstSample(int stage[][WIDTH]) {
	////////////////////////////////////////
/* ���͎󂯎�� */
////////////////////////////////////////

/* �c�Ɖ��̒��� */
	int height, width;
	cout << "���ƍ�������͂��Ă�������" << endl;
	cin >> height >> width;

	/* �Ֆ� */
	vector<string> field(height);
	cout << "�Ֆʂ���s�Â��͂��Ă�������" << endl;
	for (int h = 0; h < height; ++h) {
		cout << h << endl;
		cin >> field[h];
	}

	/* �X�^�[�g�n�_�ƃS�[���n�_ */
	int sx, sy, gx, gy;
	for (int h = 0; h < height; ++h) {
		for (int w = 0; w < width; ++w) {
			if (field[h][w] == 'S') {
				sx = h;
				sy = w;
			}
			if (field[h][w] == 'G') {
				gx = h;
				gy = w;
			}
		}
	}


	////////////////////////////////////////
	/* ���D��T���̏����ݒ� */
	////////////////////////////////////////

	// �e�Z���̍ŒZ���� (�K��Ă��Ȃ��Ƃ���� -1 �Ƃ��Ă���)
	vector<vector<int> > dist(height, vector<int>(width, -1));
	dist[sx][sy] = 0; // �X�^�[�g�� 0 ��

	// �u��x�������_�v�̂����u�܂��K��Ă��Ȃ����_�v��\���L���[
	queue<pair<int, int> > que;
	que.push(make_pair(sx, sy)); // �X�^�[�g�� push


	////////////////////////////////////////
	/* ���D��T�������{ */
	////////////////////////////////////////    

	/* �L���[����ɂȂ�܂� */
	while (!que.empty()) {
		pair<int, int> current_pos = que.front(); // �L���[�̐擪������ (C++ �ł͂�������Ă� pop ���Ȃ�)
		int x = current_pos.first;
		int y = current_pos.second;
		que.pop(); // �L���[���� pop ��Y�ꂸ��

		// �אڒ��_��T��
		for (int direction = 0; direction < 4; ++direction) {
			int next_x = x + dx[direction];
			int next_y = y + dy[direction];
			if (next_x < 0 || next_x >= height || next_y < 0 || next_y >= width) continue; // ��O�A�E�g�Ȃ�_��
			if (field[next_x][next_y] == '#') continue; // �ǂ̓_��

			// �܂����Ă��Ȃ����_�Ȃ� push
			if (dist[next_x][next_y] == -1) {
				que.push(make_pair(next_x, next_y));
				dist[next_x][next_y] = dist[x][y] + 1;
			}
		}
	}


	////////////////////////////////////////
	/* ���ʏo�� */
	////////////////////////////////////////    

	/* �e�}�X�ւ̃X�^�[�g����̍ŒZ���������Ă݂� */
	for (int h = 0; h < height; ++h) {
		for (int w = 0; w < width; ++w) {
			if (field[h][w] != '.' && field[h][w] != 'G') cout << std::setw(3) << field[h][w];
			else cout << std::setw(3) << dist[h][w];
		}
		cout << endl;
	}
	cout << endl;

	//////////////////////////////////////////
	///* ���� */
	//////////////////////////////////////////
	//SQUARE position_box;
	//SQUARE position_goal;
	////int N = countSpace(stage);//���_�̐�
	////int M = countConnectionAll(stage);//�ӂ̐�
	////TargetStage Target(N);

	// /* �Ֆ� */
	//vector<vector<string>> field(WIDTH);
	///*for (int y = 0; y < HEIGHT; y++) {
	//	for (int x = 0; x < WIDTH; x++) {
	//		if(stage[y][x]==WALL)
	//			field[x][y] = '#';
	//		else if (stage[y][x] == MAN)
	//			field[x][y] = '@';
	//		else if (stage[y][x] == MAN_ON_GOAL)
	//			field[x][y] = '+';
	//		else if (stage[y][x] == BOX)
	//			field[x][y] = '$';
	//		else if (stage[y][x] == BOX_ON_GOAL)
	//			field[x][y] = '*';
	//		else if (stage[y][x] == GOAL)
	//			field[x][y] = '.';
	//		else if (stage[y][x] == PATH)
	//			field[x][y] = ' ';
	//		else if (stage[y][x] == CHECK)
	//			field[x][y] = 'x';
	//	}
	//}*/
	//for (int h = 0; h < HEIGHT; ++h) cin >> field[h];


	///* �X�^�[�g�n�_�ƃS�[���n�_ */
	//position_box = searchBox(stage);
	//position_goal = searchGoal(stage);
	//int sx, sy, gx, gy;
	//sx = position_box.y;
	//sy = position_box.x;
	//gx = position_goal.y;
	//gy = position_goal.x;
	//////////////////////////////////////////
	///* ���D��T���̏����ݒ� */
	//////////////////////////////////////////
	//// �e�Z���̍ŒZ���� (�K��Ă��Ȃ��Ƃ���� -1 �Ƃ��Ă���)
	//vector<vector<int> > dist(HEIGHT, vector<int>(WIDTH, -1));
	//dist[sx][sy] = 0; // �X�^�[�g�� 0 ��

	//// �T�����Ɋe�}�X�͂ǂ̃}�X���痈���̂���\���z�� (�ŒZ�o�H����m�邾���Ȃ�A����͕s�v)
	//vector<vector<int> > prev_x(WIDTH, vector<int>(HEIGHT, -1));
	//vector<vector<int> > prev_y(WIDTH, vector<int>(HEIGHT, -1));

	//// �u��x�������_�v�̂����u�܂��K��Ă��Ȃ����_�v��\���L���[
	//queue<pair<int, int> > que;
	//que.push(make_pair(sx, sy)); // �X�^�[�g�� push

	//////////////////////////////////////////
	///* ���D��T�������{ */
	//////////////////////////////////////////    

	// /* �L���[����ɂȂ�܂� */
	//while (!que.empty()) {
	//	pair<int, int> current_pos = que.front(); // �L���[�̐擪������ (C++ �ł͂�������Ă� pop ���Ȃ�)
	//	int x = current_pos.first;
	//	int y = current_pos.second;
	//	que.pop(); // �L���[���� pop ��Y�ꂸ��

	//	// �אڒ��_��T��
	//	for (int direction = 0; direction < 4; ++direction) {
	//		int next_x = x + dx[direction];
	//		int next_y = y + dy[direction];
	//		if (next_x < 0 || next_x >= HEIGHT || next_y < 0 || next_y >= WIDTH) continue; // ��O�A�E�g�Ȃ�_��
	//		if (field[next_x][next_y] == "#") continue; // �ǂ̓_��

	//		// �܂����Ă��Ȃ����_�Ȃ� push
	//		if (dist[next_x][next_y] == -1) {
	//			que.push(make_pair(next_x, next_y));
	//			dist[next_x][next_y] = dist[x][y] + 1; // (next_x, next_y) �̋������X�V
	//			prev_x[next_x][next_y] = x;  // �ǂ̒��_�����񂪓`�d���ė������A�c�����̍��W������
	//			prev_y[next_x][next_y] = y;  // �ǂ̒��_�����񂪓`�d���ė������A�������̍��W������
	//		}
	//	}
	//}


	//////////////////////////////////////////
 //  /* ���ʏo�� */
 //  ////////////////////////////////////////    

 //  /* �e�}�X�ւ̃X�^�[�g����̍ŒZ���������Ă݂� */
	//for (int h = 0; h < HEIGHT; ++h) {
	//	for (int w = 0; w < WIDTH; ++w) {
	//		if (field[h][w] != '.') cout << std::setw(3) << field[h][w];
	//		else cout << std::setw(3) << dist[h][w];
	//	}
	//	cout << endl;
	//}
	//cout << endl;

	///* �S�[���Ɏ���܂ł̍ŒZ�o�H�𕜌����Ă݂� */
	//int x = gx, y = gy;
	//while (x != -1 && y != -1) {
	//	field[x][y] = 'o'; // �ʉ߂������Ƃ�����

	//	// �O�̒��_�֍s��
	//	int px = prev_x[x][y];
	//	int py = prev_y[x][y];
	//	x = px, y = py;
	//}
	//for (int h = 0; h < HEIGHT; ++h) {
	//	for (int w = 0; w < WIDTH; ++w) {
	//		cout << std::setw(3) << field[h][w];
	//	}
	//	cout << endl;
	//}

	return 0;
}

int searchBreadthFirst(int stage[][WIDTH]) {
	
	return 0;
}