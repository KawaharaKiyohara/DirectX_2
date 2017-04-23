/*!
 * @brief	�Q�[���N���X�B
 */

#pragma once

#include "Player.h"
#include "GameCamera.h"
#include "Map.h"

class Enemy;
class Bullet;

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
	const Camera& GetCamera() const
	{
		return gameCamera.GetCamera();
	}
	/*!
	* @brief	�Q�[���J�������擾�B
	*/
	const GameCamera& GetGameCamera() const
	{
		return gameCamera;
	}
	/*!
	* @brief	�e�ۂ�ǉ��B�B
	*/
	void AddBullet(Bullet* bl)
	{
		bulletList.push_back(bl);
	}
	/*!
	* @brief	�e�ۂ̃��X�g���擾�B
	*/
	const std::list<Bullet*>& GetBulletList()
	{
		return bulletList;
	}
private:
	GameCamera gameCamera;			//�Q�[���J�����B
	Player player;					//�v���C���[�B
	std::list<Enemy*> enemyList;	//�G�B
	std::list<Bullet*> bulletList;	//�e�ۂ̃��X�g�B
	Map map;						//�n�`�B
};

extern Game* game;