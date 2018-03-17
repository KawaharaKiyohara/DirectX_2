#include "stdafx.h"
#include "Enemy.h"
#include "game.h"
#include "bullet.h"
#include "enemyBullet.h"

SkinModelData* Enemy::modelData = NULL;

Enemy::Enemy()
{
}


Enemy::~Enemy()
{
	delete modelData;
	modelData = NULL;
}
void Enemy::Start(const D3DXVECTOR3& pos)
{
	position = pos;
	//ライトを初期化。
	light.SetDiffuseLightDirection(0, D3DXVECTOR4(0.707f, 0.0f, -0.707f, 1.0f));
	light.SetDiffuseLightDirection(1, D3DXVECTOR4(-0.707f, 0.0f, -0.707f, 1.0f));
	light.SetDiffuseLightDirection(2, D3DXVECTOR4(0.0f, 0.707f, -0.707f, 1.0f));
	light.SetDiffuseLightDirection(3, D3DXVECTOR4(0.0f, -0.707f, -0.707f, 1.0f));

	light.SetDiffuseLightColor(0, D3DXVECTOR4(0.2f, 0.2f, 0.2f, 1.0f));
	light.SetDiffuseLightColor(1, D3DXVECTOR4(0.2f, 0.2f, 0.2f, 1.0f));
	light.SetDiffuseLightColor(2, D3DXVECTOR4(0.3f, 0.3f, 0.3f, 1.0f));
	light.SetDiffuseLightColor(3, D3DXVECTOR4(0.2f, 0.2f, 0.2f, 1.0f));
	light.SetAmbientLight(D3DXVECTOR4(0.3f, 0.3f, 0.3f, 1.0f));

	if (modelData == NULL) {
		//モデルをロード。
		modelData = new SkinModelData;
		modelData->LoadModelData("Assets/model/DarkUnity.X", NULL);
	}

	model.Init(modelData);
	model.SetLight(&light);
}
bool Enemy::Update()
{
	timer++;
	if (timer > 20) {
		timer = 0;
		EnemyBullet* enemyBullet = new EnemyBullet();
		enemyBullet->Start(
			position, 
			D3DXVECTOR3(0.0f, -0.1f, 0.0f)
		);
		game->AddEnemyBullets(enemyBullet);
	}
	auto& bulletList = game->GetPlayerBullet();
	for (auto& bullet : bulletList) {
		D3DXVECTOR3 diff = 
			bullet->GetPosition() - position;
		float len = D3DXVec3Length(&diff);
		if (len < 0.5f) {
			//弾と衝突している
			return false;
		}
	}
	//Question 1 死亡判定を行う。
	//①弾丸をリストを取ってくる必要がある。
	//  game.hのGetPlayerBullet関数にサンプルがあるよ。
	//②弾丸とエネミーの距離を調べる必要があります。
	//	bulletクラスにはpositionのメンバがあるけど
	//  privateになっている。
	//③死亡するときに戻り値をfalseにする。
	//Question 2 エネミーも球を発射できるようにする。
	return true;
}
void Enemy::Render()
{
	model.UpdateWorldMatrix(position, D3DXQUATERNION(0.0f, 0.0f, 0.0f, 1.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	Camera* camera = game->GetCamera();
	model.Draw(&camera->GetViewMatrix(), &camera->GetProjectionMatrix());
}
