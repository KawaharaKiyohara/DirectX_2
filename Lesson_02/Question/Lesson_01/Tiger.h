#pragma once
#include "Model.h"
class Tiger
{
public:
	/*!
	 * @brief	コンストラクタ。
	 */
	Tiger();
	/*!
	 * @brief	デストラクタ。
	 */
	~Tiger();
	/*!
	 * @brief	初期化。
	 */
	void Init();
	/*!
	 * @brief	更新処理。
	 */
	void Update();
	/*!
	 * @brief	描画処理。
	 */
	void Render();
	/*!
	 * @brief	開放。
	 */
	void Release();
	/*!
	 * @brief	座標の設定。
	 */
	void SetPosition(D3DXVECTOR3 pos);
private:
	/*!
	 * @brief	移動処理。
	 */
	void Move();
private:
	D3DXVECTOR3		position;				//座標。
	Model model;
};

