#pragma once
#include "Model.h"

class Tiger : public Model
{
public:
	/*!
	 * @brief	�R���X�g���N�^�B
	 */
	Tiger();
	/*!
	 * @brief	�f�X�g���N�^�B
	 */
	~Tiger();
	/*!
	 * @brief	�X�V�����B
	 */
	void Update();
	/*!
	 * @brief	���W�̐ݒ�B
	 */
	void SetPosition(D3DXVECTOR3 pos);
private:

	/*!
	 * @brief	�ړ������B
	 */
	void Move();
private:
	D3DXVECTOR3 position;
};

