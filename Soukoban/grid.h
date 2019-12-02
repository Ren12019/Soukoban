#pragma once
#include "define.h"

class Grid
{
public:
	Grid();//�R���X�g���N�^
	~Grid();//�f�X�g���N�^

	int grid[GRID_SIZE][GRID_SIZE] = {};//�X�e�[�W�̂��ƂƂȂ�O���b�h
	void setGrid();//�����ŃO���b�h���̔z�u�����肷��
	void setGridTemplate();//�e���v���[�g�̔z�u�𗘗p���O���b�h���쐬����
private:
	void rotateGrid();//�O���b�h����]������
	void flipGrid();//���b�h�𔽓]������
	void printGrid();//�O���b�h��\������
};