#include "stdafx.h"
#include "GameCamera.h"
#include "Player.h"
#include "lib/Input.h"

GameCamera::GameCamera() :
	toEyePos(0.0f, 0.0f, 0.0f, 1.0f),
	targetOffset(0.0f, 0.0f, 0.0f, 1.0f),
	cameraDir(0.0f, 0.0f, 0.0f)
{
}
GameCamera::~GameCamera()
{
}
void GameCamera::Start(Player* player)
{
	toEyePos.z = -4.0f;
	toEyePos.y = 0.5f;
	targetOffset.y = 1.0f;
	targetOffset.x = 0.0f;
	camera.SetAspect(1280.0f / 720.0f);
	camera.Init();
	this->player = player;
	UpdateCamera();
}
//カメラを更新。
void GameCamera::UpdateCamera()
{
	D3DXVECTOR3 target;

	target = player->GetPosition();
	target.x += targetOffset.x;
	target.y += targetOffset.y;
	target.z += targetOffset.z;
	D3DXVECTOR3 eyePos = target;
	eyePos.x += toEyePos.x;
	eyePos.y += toEyePos.y;
	eyePos.z += toEyePos.z;

	camera.SetEyePt(eyePos);
	camera.SetLookatPt(target);
	camera.Update();
}
void GameCamera::PreUpdate()
{
	if (GetInput().IsMouseMove()) {
		//マウスが動いた。
		D3DXVECTOR2 mouseMove = GetInput().GetMouseMove();
		//テスト
		D3DXMATRIX mRot;
		D3DXMatrixRotationY(&mRot, (mouseMove.x / WINDOW_WIDTH) * 1.0f);
		D3DXVec4Transform(&toEyePos, &toEyePos, &mRot);
	}
	cameraDir = D3DXVECTOR3(toEyePos);
	cameraDir.y = 0.0f;
	D3DXVec3Normalize(&cameraDir, &cameraDir);
}
void GameCamera::Update()
{
	UpdateCamera();
}