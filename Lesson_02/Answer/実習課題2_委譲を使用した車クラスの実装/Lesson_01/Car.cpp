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
	model.UpdateWorldMatrix(position);
}
/*!
* @brief	初期化処理。
*/
void Car::Init()
{
	model.Init("car.x");
}
/*!
* @brief	描画処理。
*/
void Car::Render()
{
	model.Render();
}
/*!
* @brief	開放処理。
*/
void Car::Release()
{
	model.Release();
}
/*!
* @brief	座標の設定。
*/
void Car::SetPosition(D3DXVECTOR3 pos)
{
	position = pos;
}
