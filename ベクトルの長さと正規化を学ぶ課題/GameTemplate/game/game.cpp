/*!
 * @brief	�Q�[��
 */
#include "stdafx.h"
#include "game.h"
#include "Enemy.h"
#include "Bullet.h"

/*!
 * @brief	�R���X�g���N�^�B
 */
Game::Game()
{
}
/*!
 * @brief	�f�X�g���N�^�B
 */
Game::~Game()
{
	for (auto enemy : enemyList) {
		delete enemy;
	}
	for (auto bullet : playerBullets) {
		delete bullet;
	}
}
/*!
 * @brief	�Q�[�����N�����Ă����x�����Ă΂��֐��B
 */
void Game::Start()
{
	//�J�����������B
	camera.Init();
	camera.SetEyePt(D3DXVECTOR3(0.0f, 3.0f, 10.0f));
	camera.SetLookatPt(D3DXVECTOR3(0.0f, 3.0f, 0.0f));
	camera.Update();

	player.Start();
	//�_�[�N���j�e�B������z�u
	for (int i = 0; i < 10; i++) {
		Enemy* enemy = new Enemy;
		enemyList.push_back(enemy);
		
		enemy->Start(D3DXVECTOR3(-3.5f + 0.8f * i, 6.0f, 0.0f));
	}

}
/*!
 * @brief	�X�V�B
 */
void Game::Update()
{
	auto enemyIt = enemyList.begin();
	while(enemyIt != enemyList.end()){	
		if (!(*enemyIt)->Update()) {
			//���S�B
			enemyIt = enemyList.erase(enemyIt);
		}
		else {
			enemyIt++;
		}
	}
	auto bulletIt = playerBullets.begin();
	while( bulletIt != playerBullets.end()) {
		if (!(*bulletIt)->Update()) {
			//���S
			bulletIt = playerBullets.erase(bulletIt);
		}
		else {
			bulletIt++;
		}
	}
	player.Update();
	camera.Update();
}
/*!
 * @brief	�`��B
 */
void Game::Render()
{
	for (auto enemy : enemyList) {
		enemy->Render();
	}
	for (auto bullet : playerBullets) {
		bullet->Render();
	}
	player.Render();
}
