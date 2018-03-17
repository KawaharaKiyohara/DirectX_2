/*!
 * @brief	�Q�[���N���X�B
 */

#pragma once

#include "Player.h"

class Enemy;
class Bullet;
class EnemyBullet;

/*!
 * @brief	�Q�[���N���X�B
 */
class Game{
public:
	/*!
	 * @brief	�R���X�g���N�^�B
	 */
	Game();
	/*!
	 * @brief	�f�X�g���N�^�B
	 */
	~Game();
	/*!
	 * @brief	�Q�[�����N�����Ă����x�����Ă΂��֐��B
	 */
	void Start();
	/*!
	 * @brief	�X�V�B
	 */
	void Update();
	/*!
	 * @brief	�`��B
	 */
	void Render();
	/*!
	* @brief	�J�������擾�B
	*/
	Camera* GetCamera()
	{
		return &camera;
	}
	/*!
	* @brief	�v���C���[���������e��ǉ��B
	*/
	void AddPlayerBullets(Bullet* bullet)
	{
		playerBullets.push_back(bullet);
	}
	void AddEnemyBullets(EnemyBullet* bullet)
	{
		enemybullets.push_back(bullet);
	}
	/*!
	* @brief	�v���C���[���������e�̃��X�g���擾�B
	* @code
		�T���v���R�[�h
		auto& bulletList = game->GetPlayerBullet();
		for(auto bullet : bulletList){
			bullet->Render();
        }
 	  @endcode
	*/
	const std::list<Bullet*>& GetPlayerBullet()
	{
		return playerBullets;
	}
	Player& GetPlayer()
	{
		return player;
	}
private:
	Camera camera;
	Player player;
	std::list<Enemy*> enemyList;			//�G
	std::list<Bullet*> playerBullets;		//�v���C���[�����˂����e�B
	std::list<EnemyBullet*> enemybullets;	//�G�����˂����e�B
};

extern Game* game;