/*!
 * @brief	ゲーム
 */
#include "stdafx.h"
#include "game.h"
#include <time.h>
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
}
/*!
 * @brief	ゲームが起動してから一度だけ呼ばれる関数。
 */
void Game::Start()
{
	srand((unsigned int)time(NULL));
	player.Start();
	gameCamera.Start(&player);
	map.Start();
}
/*!
 * @brief	更新。
 */
void Game::Update()
{
	map.Update();
	gameCamera.PreUpdate();
	player.Update();
	gameCamera.Update();
}
/*!
 * @brief	描画。
 */
void Game::Render()
{
	player.Render();
	map.Render();
}
