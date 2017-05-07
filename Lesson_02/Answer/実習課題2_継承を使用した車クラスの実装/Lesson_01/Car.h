#pragma once
#include "Model.h"
class Car : public Model
{
public:
	Car();
	~Car();
	/*!
	* @brief	更新処理。
	*/
	void Update();
	/*!
	* @brief	座標の設定。
	*/
	void SetPosition(D3DXVECTOR3 pos);
private:
	D3DXVECTOR3 position;
};

