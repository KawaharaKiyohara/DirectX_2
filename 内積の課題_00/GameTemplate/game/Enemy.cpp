#include "stdafx.h"
#include "Enemy.h"
#include "game.h"
#include "Bullet.h"

namespace {
	const float PI = 3.14159265358979323846f;
}
SkinModelData* Enemy::modelData = NULL;

Enemy::Enemy() :
	direction(0.0f, 0.0f, 1.0f)
{
}


Enemy::~Enemy()
{
}
void Enemy::Start(const D3DXVECTOR3& pos)
{
	state = eState_Search;
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

	float angle = ((rand() % 100) - 50) / 50.0f;
	angle *= PI;
	D3DXMATRIX mRot;
	D3DXMatrixRotationY(&mRot, angle);
	direction.x = mRot.m[2][0];
	direction.y = mRot.m[2][1];
	direction.z = mRot.m[2][2];
	D3DXQuaternionRotationAxis(&rotation, &D3DXVECTOR3(0.0f, 1.0f, 0.0f), angle);

}
bool Enemy::Update()
{
	moveFrameCount++;
	if (state == eState_Search) {
		const std::list<Bullet*>& bulletList = game->GetBulletList();
		for (auto bullet : bulletList) {
			D3DXVECTOR3 diff = bullet->GetPosition() - position;
			if (D3DXVec3Length(&diff) < 1.0f) {
				return false;
			}
		}
		//探索状態。
		if (moveFrameCount % 120 == 0) {
			//方向転換
			direction *= -1.0f;
			D3DXQUATERNION qAdd;
			D3DXQuaternionRotationAxis(&qAdd, &D3DXVECTOR3(0.0f, 1.0f, 0.0f), PI);
			rotation *= qAdd;
		}
		//Question ① ここに視野角の判定を記述する。
		position += direction * 0.05f;

	}
	else if (state == eState_Find) {
		//発見状態。
	}
	return true;
}
void Enemy::Render()
{
	model.UpdateWorldMatrix(position, rotation, D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	const Camera& camera = game->GetCamera();
	model.Draw(&camera.GetViewMatrix(), &camera.GetProjectionMatrix());
}
