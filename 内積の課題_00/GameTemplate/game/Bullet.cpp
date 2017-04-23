#include "stdafx.h"
#include "Bullet.h"
#include "game.h"

SkinModelData* Bullet::modelData = NULL;

Bullet::Bullet()
{
}


Bullet::~Bullet()
{
	//delete modelData;
	//modelData = NULL;
}

void Bullet::Start(const D3DXVECTOR3& pos, const D3DXVECTOR3& moveSpeed)
{
	this->moveSpeed = moveSpeed;
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
		modelData->LoadModelData("Assets/model/bullet.x", NULL);
	}

	model.Init(modelData);
	model.SetLight(&light);
	life = 60;

}
bool Bullet::Update()
{
	life--;
	if (life < 0) {
		//死亡。
		return false;
	}
	float deltaTime = 1.0f / 60.0f;
	D3DXVECTOR3 gravity(0.0f, -9.8f, 0.0f);
	D3DXVECTOR3 accel = gravity * deltaTime;
	moveSpeed += accel;
	position += moveSpeed * deltaTime;
	return true;
}
void Bullet::Render()
{
	model.UpdateWorldMatrix(position, D3DXQUATERNION(0.0f, 0.0f, 0.0f, 1.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	const Camera& camera = game->GetCamera();
	model.Draw(&camera.GetViewMatrix(), &camera.GetProjectionMatrix());
}
