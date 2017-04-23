/*!
 * @brief	ゲーム
 */
#include "stdafx.h"
#include "game.h"
#include "Enemy.h"
#include "Bullet.h"

/*!
 * @brief	コンストラクタ。
 */
Game::Game()
{
}
/*!
 * @brief	デストラクタ。
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
 * @brief	ゲームが起動してから一度だけ呼ばれる関数。
 */
void Game::Start()
{
	//カメラ初期化。
	camera.Init();
	camera.SetEyePt(D3DXVECTOR3(0.0f, 3.0f, 10.0f));
	camera.SetLookatPt(D3DXVECTOR3(0.0f, 3.0f, 0.0f));
	camera.Update();

	player.Start();
	//ダークユニティちゃんを配置
	for (int i = 0; i < 10; i++) {
		Enemy* enemy = new Enemy;
		enemyList.push_back(enemy);
		
		enemy->Start(D3DXVECTOR3(-3.5f + 0.8f * i, 6.0f, 0.0f));
	}

}
/*!
 * @brief	更新。
 */
void Game::Update()
{
	auto enemyIt = enemyList.begin();
	while(enemyIt != enemyList.end()){	
		if (!(*enemyIt)->Update()) {
			//死亡。
			enemyIt = enemyList.erase(enemyIt);
		}
		else {
			enemyIt++;
		}
	}
	auto bulletIt = playerBullets.begin();
	while( bulletIt != playerBullets.end()) {
		if (!(*bulletIt)->Update()) {
			//死亡
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
 * @brief	描画。
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
