/*!
 * @brief	�Q�[��
 */
#include "stdafx.h"
#include "game.h"
#include <time.h>
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
}
/*!
 * @brief	�`��B
 */
void Game::Render()
{
	player.Render();
	map.Render();
}
