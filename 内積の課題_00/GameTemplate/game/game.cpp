/*!
 * @brief	�Q�[��
 */
#include "stdafx.h"
#include "game.h"
#include "Enemy.h"
#include <time.h>
#include "Bullet.h"

const int NUM_ENEMY = 20;
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
	for (auto bullet : bulletList) {
		delete bullet;
	}
}
/*!
 * @brief	�Q�[�����N�����Ă����x�����Ă΂��֐��B
 */
void Game::Start()
{
	srand((unsigned int)time(NULL));
	player.Start();
	gameCamera.Start(&player);
	map.Start();

	for (int i = 0; i < NUM_ENEMY; i++) {
		float t_x = ((rand() % 100) - 50) / 50.0f;
	//	float t_y = ((rand() % 100) - 50) / 50.0f;
		float t_z = ((rand() % 100) - 50) / 50.0f;
		Enemy* enemy = new Enemy;
		enemy->Start(D3DXVECTOR3(t_x * 30.0f, 0.0f, t_z * 30.0f));
		enemyList.push_back(enemy);
	}
}
/*!
 * @brief	�X�V�B
 */
void Game::Update()
{
	map.Update();
	gameCamera.PreUpdate();
	player.Update();
	gameCamera.Update();
	for (auto enemyIt = enemyList.begin(); enemyIt != enemyList.end(); ) {
		if (!(*enemyIt)->Update()) {
			delete *enemyIt;
			enemyIt = enemyList.erase(enemyIt);
		}
		else {
			enemyIt++;
		}
	}
	for (auto bulletIt = bulletList.begin(); bulletIt != bulletList.end();) {
		if (!(*bulletIt)->Update()) {
			delete *bulletIt;
			bulletIt = bulletList.erase(bulletIt);
		}
		else {
			bulletIt++;
		}
	}
}
/*!
 * @brief	�`��B
 */
void Game::Render()
{
	for (auto enemy : enemyList) {
		enemy->Render();
	}
	player.Render();
	map.Render();
	for (auto bullet : bulletList) {
		bullet->Render();
	}
}
