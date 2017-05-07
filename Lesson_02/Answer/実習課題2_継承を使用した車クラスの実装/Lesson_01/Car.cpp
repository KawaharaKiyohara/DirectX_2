#include "stdafx.h"
#include "Car.h"


Car::Car()
{
}


Car::~Car()
{
}
/*!
* @brief	更新処理。
*/
void Car::Update()
{
	UpdateWorldMatrix(position);
}
/*!
* @brief	座標の設定。
*/
void Car::SetPosition(D3DXVECTOR3 pos)
{
	position = pos;
}
