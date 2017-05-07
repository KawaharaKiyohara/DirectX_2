#pragma once
#include "Model.h"
class Car 
{
public:
	Car();
	~Car();
	/*!
	* @brief	更新処理。
	*/
	void Update();
	/*!
	* @brief	初期化処理。
	*/
	void Init();
	/*!
	* @brief	描画処理。
	*/
	void Render();
	/*!
	* @brief	開放処理。
	*/
	void Release();
	/*!
	* @brief	座標の設定。
	*/
	void SetPosition(D3DXVECTOR3 pos);
private:
	D3DXVECTOR3 position;
	Model model;
};

