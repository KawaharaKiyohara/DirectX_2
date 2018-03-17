/*!
 * @brief	ゲームクラス。
 */

#pragma once

#include "Player.h"
#include "GameCamera.h"
#include "Map.h"
#include "CourceDef.h"

class Enemy;
class Bullet;

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
	/*!
	* @brief	カメラを取得。
	*/
	const Camera& GetCamera() const
	{
		return gameCamera.GetCamera();
	}
	/*!
	* @brief	ゲームカメラを取得。
	*/
	const GameCamera& GetGameCamera() const
	{
		return gameCamera;
	}

private:
	GameCamera	gameCamera;			//ゲームカメラ。
	Player		player;				//プレイヤー。
	Map			map;				//地形。

};

extern Game* game;