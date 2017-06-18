/*!
 * @brief	�Q�[���N���X�B
 */

#pragma once


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
private:
	D3DXVECTOR3		toEyePos;		//�����_���王�_�܂ł̃x�N�g���B
	Camera camera;
	SkinModel model;
	SkinModelData modelData;
	Animation animation;
	Light light;
};

extern Game* game;