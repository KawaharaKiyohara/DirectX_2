#include "stdafx.h"
#include "Car.h"


Car::Car()
{
}


Car::~Car()
{
}
/*!
* @brief	�X�V�����B
*/
void Car::Update()
{
	UpdateWorldMatrix(position);
}
/*!
* @brief	���W�̐ݒ�B
*/
void Car::SetPosition(D3DXVECTOR3 pos)
{
	position = pos;
}
