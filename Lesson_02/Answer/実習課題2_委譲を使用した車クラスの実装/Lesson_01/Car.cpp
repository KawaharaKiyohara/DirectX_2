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
	model.UpdateWorldMatrix(position);
}
/*!
* @brief	�����������B
*/
void Car::Init()
{
	model.Init("car.x");
}
/*!
* @brief	�`�揈���B
*/
void Car::Render()
{
	model.Render();
}
/*!
* @brief	�J�������B
*/
void Car::Release()
{
	model.Release();
}
/*!
* @brief	���W�̐ݒ�B
*/
void Car::SetPosition(D3DXVECTOR3 pos)
{
	position = pos;
}
