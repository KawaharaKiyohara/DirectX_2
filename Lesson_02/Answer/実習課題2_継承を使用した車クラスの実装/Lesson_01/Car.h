#pragma once
#include "Model.h"
class Car : public Model
{
public:
	Car();
	~Car();
	/*!
	* @brief	�X�V�����B
	*/
	void Update();
	/*!
	* @brief	���W�̐ݒ�B
	*/
	void SetPosition(D3DXVECTOR3 pos);
private:
	D3DXVECTOR3 position;
};

