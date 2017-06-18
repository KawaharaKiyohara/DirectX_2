/*!
 * @brief	ゲームクラス。
 */

#pragma once


/*!
 * @brief	ゲームクラス。
 */
class Game{
public:
	/*!
	 * @brief	コンストラクタ。
	 */
	Game();
	/*!
	 * @brief	デストラクタ。
	 */
	~Game();
	/*!
	 * @brief	ゲームが起動してから一度だけ呼ばれる関数。
	 */
	void Start();
	/*!
	 * @brief	更新。
	 */
	void Update();
	/*!
	 * @brief	描画。
	 */
	void Render();
private:
	D3DXVECTOR3		toEyePos;		//注視点から視点までのベクトル。
	Camera camera;
	SkinModel model;
	SkinModelData modelData;
	Animation animation;
	Light light;
	Pad pad;		//ゲームパッド。
};

extern Game* game;