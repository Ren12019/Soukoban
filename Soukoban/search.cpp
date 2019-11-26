#include "search.h"
#include <vector>
#include <queue>
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

const int dx[4] = { 0, 1, 0, -1 };
const int dy[4] = { 1, 0, -1, 0 };

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

int searchBreadthFirst(int stage[][WIDTH]) {
	////////////////////////////////////////
	/* ���� */
	////////////////////////////////////////
	SQUARE position_box;
	SQUARE position_goal;
	//int N = countSpace(stage);//���_�̐�
	//int M = countConnectionAll(stage);//�ӂ̐�
	//TargetStage Target(N);

	 /* �Ֆ� */
	/*vector<int> field(WIDTH);
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			field[(y*WIDTH) + x] = stage[y][x];
		}
	}*/

	/* �X�^�[�g�n�_�ƃS�[���n�_ */
	position_box = searchBox(stage);
	position_goal = searchGoal(stage);
	////////////////////////////////////////
	/* ���D��T���̏����ݒ� */
	////////////////////////////////////////
	// �e�Z���̍ŒZ���� (�K��Ă��Ȃ��Ƃ���� -1 �Ƃ��Ă���)
	vector<vector<int> > dist(HEIGHT, vector<int>(WIDTH, -1));
	dist[position_box.y][position_box.x] = 0; // �X�^�[�g�� 0 ��

	// �T�����Ɋe�}�X�͂ǂ̃}�X���痈���̂���\���z�� (�ŒZ�o�H����m�邾���Ȃ�A����͕s�v)
	vector<vector<int> > prev_x(WIDTH, vector<int>(HEIGHT, -1));
	vector<vector<int> > prev_y(WIDTH, vector<int>(HEIGHT, -1));

	// �u��x�������_�v�̂����u�܂��K��Ă��Ȃ����_�v��\���L���[
	queue<pair<int, int> > que;
	que.push(make_pair(position_box.y, position_box.x)); // �X�^�[�g�� push

	////////////////////////////////////////
	/* ���D��T�������{ */
	////////////////////////////////////////    

	/* �L���[����ɂȂ�܂� */
	while (!que.empty()) {
		pair<int, int> current_pos = que.front(); // �L���[�̐擪������ (C++ �ł͂�������Ă� pop ���Ȃ�)
		int x = current_pos.second;
		int y = current_pos.first;
		que.pop(); // �L���[���� pop ��Y�ꂸ��

		// �אڒ��_��T��
		for (int direction = 0; direction < 4; ++direction) {
			int next_x = x + dx[direction];
			int next_y = y + dy[direction];
			if (next_x < 0 || next_x >= WIDTH || next_y < 0 || next_y >= HEIGHT) continue; // ��O�A�E�g�Ȃ�_��
			if (stage[next_y][next_x] == WALL) continue; // �ǂ̓_��

			// �܂����Ă��Ȃ����_�Ȃ� push
			if (dist[next_y][next_x] == -1) {
				que.push(make_pair(next_y, next_x));
				dist[next_y][next_x] = dist[y][x] + 1; // (next_x, next_y) �̋������X�V
				prev_x[next_y][next_x] = x;  // �ǂ̒��_�����񂪓`�d���ė������A�c�����̍��W������
				prev_y[next_y][next_x] = y;  // �ǂ̒��_�����񂪓`�d���ė������A�������̍��W������
			}
		}
	}

	////////////////////////////////////////
   /* ���ʏo�� */
   ////////////////////////////////////////    

   /* �e�}�X�ւ̃X�^�[�g����̍ŒZ���������Ă݂� */
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			if (stage[y][x] == WALL) printf("#");
			else printf("%d",dist[y][x]);
		}
		printf("\n");
	}
	printf("\n");

	/* �S�[���Ɏ���܂ł̍ŒZ�o�H�𕜌����Ă݂� */
	int x =position_goal.x, y = position_goal.y;
	while (x != -1 && y != -1) {
		stage[y][x] = CHECK; // �ʉ߂������Ƃ�����

		// �O�̒��_�֍s��
		int px = prev_x[x][y];
		int py = prev_y[x][y];
		x = px, y = py;
	}
	
	printStage(stage);

	return 0;
}