// StructSample.cpp : �R���\�[�� �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

#include "stdafx.h"
#include <iostream>

int g_playerHp = 100;		//�v���C���[��HP
int g_playerMp = 200;		//�v���C���[��MP
int g_playerLv = 10;		//�v���C���[�̃��x��
int g_playerAtk = 20;		//�v���C���[�̍U���́B
int g_playerDef = 30;		//�v���C���[�̖h��́B

int main()
{
	std::cout << "�v���C���[��HP      " << g_playerHp << "\n";
	std::cout << "�v���C���[��MP      " << g_playerMp << "\n";
	std::cout << "�v���C���[��Lv      " << g_playerLv << "\n";
	std::cout << "�v���C���[�̍U����  " << g_playerAtk << "\n";
	std::cout << "�v���C���[�̖h���  " << g_playerDef << "\n";
    return 0;
}

