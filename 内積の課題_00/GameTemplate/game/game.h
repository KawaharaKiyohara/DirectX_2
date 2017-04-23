/*!
 * @brief	ゲームクラス。
 */

#pragma once

#include "Player.h"
#include "GameCamera.h"
#include "Map.h"

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
	/*!
	* @brief	弾丸を追加。。
	*/
	void AddBullet(Bullet* bl)
	{
		bulletList.push_back(bl);
	}
	/*!
	* @brief	弾丸のリストを取得。
	*/
	const std::list<Bullet*>& GetBulletList()
	{
		return bulletList;
	}
private:
	GameCamera gameCamera;			//ゲームカメラ。
	Player player;					//プレイヤー。
	std::list<Enemy*> enemyList;	//敵。
	std::list<Bullet*> bulletList;	//弾丸のリスト。
	Map map;						//地形。
};

extern Game* game;