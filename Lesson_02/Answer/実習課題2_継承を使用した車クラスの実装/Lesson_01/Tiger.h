#pragma once
#include "Model.h"

class Tiger : public Model
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
	 * @brief	更新処理。
	 */
	void Update();
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
	D3DXVECTOR3 position;
};

