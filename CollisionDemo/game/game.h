/*!
 * @brief	�Q�[���N���X�B
 */

#pragma once

#include "Player.h"
#include "Map.h"
#include "myEngine/Physics/Physics.h"
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
	//�Q�[���J�����̃C���X�^���X���擾�B
	Camera* GetCamera()
	{
		return &camera;
	}

private:
	D3DXVECTOR3 toCameraPos;
	Camera camera;
	Player player;
	Map map;
};

extern Game* game;