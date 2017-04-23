#include "stdafx.h"
#include "Player.h"
#include "game.h"
#include "lib/Input.h"
#include "Bullet.h"

Player::Player()
{
	bulletFireInterval = 0;
	direction_z = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	direction_x = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
}


Player::~Player()
{
}

void Player::Start()
{
	//ライトを初期化。
	light.SetDiffuseLightDirection(0, D3DXVECTOR4(0.707f, 0.0f, 0.707f, 1.0f));
	light.SetDiffuseLightDirection(1, D3DXVECTOR4(-0.707f, 0.0f, 0.707f, 1.0f));
	light.SetDiffuseLightDirection(2, D3DXVECTOR4(0.0f, 0.707f, 0.707f, 1.0f));
	light.SetDiffuseLightDirection(3, D3DXVECTOR4(0.0f, -0.707f, 0.707f, 1.0f));

	light.SetDiffuseLightColor(0, D3DXVECTOR4(0.2f, 0.2f, 0.2f, 1.0f));
	light.SetDiffuseLightColor(1, D3DXVECTOR4(0.2f, 0.2f, 0.2f, 1.0f));
	light.SetDiffuseLightColor(2, D3DXVECTOR4(0.3f, 0.3f, 0.3f, 1.0f));
	light.SetDiffuseLightColor(3, D3DXVECTOR4(0.2f, 0.2f, 0.2f, 1.0f));
	light.SetAmbientLight(D3DXVECTOR4(0.3f, 0.3f, 0.3f, 1.0f));

	//モデルをロード。
	modelData.LoadModelData("Assets/model/Unity.X", &animation);

	model.Init(&modelData);
	model.SetLight(&light);
	animation.PlayAnimation(1);
	position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	rotation = D3DXQUATERNION(0.0f, 0.0f, 0.0f, 1.0f);
}
void Player::Update()
{
	animation.Update(1.0f / 60.f);
	const float moveSpeed = 0.08f;
	D3DXVECTOR3 moveDir = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	if (GetAsyncKeyState('W')) {
		moveDir -= direction_z;
	}
	if (GetAsyncKeyState('S')) {
		moveDir += direction_z;
	}
	if (GetAsyncKeyState('A')) {
		moveDir -= direction_x;
	}
	if (GetAsyncKeyState('D')) {
		moveDir += direction_x;
	}
	//弾丸を打つ
	if (GetInput().IsMouseLButtonClick()) {
		Bullet* bl = new Bullet;
		D3DXVECTOR3 bulletPos = position;
		bulletPos.y += 1.0f;
		bl->Start(bulletPos, direction_z * -40.0f);
		game->AddBullet(bl);
	}
	//カメラが向いている方向に進む。
	direction_z = game->GetGameCamera().GetCameraDir();
	D3DXVec3Normalize(&moveDir, &moveDir);
	//カメラの向いている方向と、上ベクトルとの外積を計算すると横移動のベクトルが求まる。
	D3DXVec3Cross(&direction_x, &direction_z, &D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	position += moveDir * moveSpeed;
	//回転を計算。
	float angle = acos(D3DXVec3Dot(&direction_x, &D3DXVECTOR3(1.0f, 0.0f, 0.0f)));
	D3DXVECTOR3 axis;
	D3DXVec3Cross(&axis, &direction_x, &D3DXVECTOR3(1.0f, 0.0f, 0.0f));
	D3DXQuaternionRotationAxis(&rotation, &axis, -angle);
	//ワールド行列を更新。
	model.UpdateWorldMatrix(position, rotation, D3DXVECTOR3(1.0f, 1.0f, 1.0f));
}
void Player::Render()
{
	const Camera& camera = game->GetCamera();
	model.Draw(&camera.GetViewMatrix(), &camera.GetProjectionMatrix());
}