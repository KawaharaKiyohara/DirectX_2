/*!
 * @brief	ゲーム
 */
#include "stdafx.h"
#include "game.h"


/*!
 * @brief	コンストラクタ。
 */
Game::Game()
{
}
/*!
 * @brief	デストラクタ。
 */
Game::~Game()
{
}
/*!
 * @brief	ゲームが起動してから一度だけ呼ばれる関数。
 */
void Game::Start()
{
	//カメラ初期化。
	camera.Init();
	camera.SetEyePt(D3DXVECTOR3(0.0f, 2.0f, 4.0f));
	camera.SetLookatPt(D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	toEyePos = camera.GetEyePt() - camera.GetLookatPt();
	//注視点から視点までのベクトルを計算。
	camera.Update();

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

	//モデルをロード。
	modelData.LoadModelData("Assets/model/Unity.X", &animation);
	
	model.Init(&modelData);
	model.SetLight(&light);
	animation.PlayAnimation(0);

}
/*!
 * @brief	更新。
 */
void Game::Update()
{
	//Y軸周りの回転行列を作成。
	D3DXMATRIX mRot;
	//単位行列を作成。
	D3DXMatrixIdentity(&mRot);
	if (GetAsyncKeyState(VK_LEFT)) {
		D3DXMatrixRotationY(&mRot, 0.1f);
	}
	else if (GetAsyncKeyState(VK_RIGHT)) {
		D3DXMatrixRotationY(&mRot, -0.1f);
	}
	//toEyePosを回す。
	D3DXVec3TransformCoord(&toEyePos, &toEyePos, &mRot);

	//続いて、上下に回す。
	//単位行列にしとく。
	D3DXMatrixIdentity(&mRot);
	//回転させる軸を求める。
	D3DXVECTOR3 rotAxis;
	D3DXVECTOR3 up = { 0.0f, 1.0f, 0.0f };
	D3DXVec3Cross(&rotAxis, &up, &toEyePos);
	//回転軸を正規化する。
	D3DXVec3Normalize(&rotAxis, &rotAxis);
	if (GetAsyncKeyState(VK_UP)) {
		D3DXMatrixRotationAxis(&mRot, &rotAxis, 0.1f);
	}
	else if (GetAsyncKeyState(VK_DOWN)) {
		D3DXMatrixRotationAxis(&mRot, &rotAxis, -0.1f);
	}
	
	
	//toEyePosを回す。
	D3DXVECTOR3 toEyePosOld = toEyePos;
	D3DXVec3TransformCoord(&toEyePos, &toEyePos, &mRot);
	//上下の回転は制限を設ける。
	D3DXVECTOR3 toEyePosNormalized;
	D3DXVec3Normalize(&toEyePosNormalized, &toEyePos);
	if (fabsf(toEyePosNormalized.x) < 0.5f && fabsf(toEyePosNormalized.z) < 0.5f) {
		//可動域を超えている。
		toEyePos = toEyePosOld;
	}

	//カメラの視点を決定する。
	D3DXVECTOR3 eyePos = camera.GetLookatPt();
	eyePos += toEyePos;
	camera.SetEyePt(eyePos);

	animation.Update(1.0f / 60.0f);
	camera.Update();
	model.UpdateWorldMatrix(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXQUATERNION(0.0f, 0.0f, 0.0f, 1.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f));
}
/*!
 * @brief	描画。
 */
void Game::Render()
{
	model.Draw(&camera.GetViewMatrix(), &camera.GetProjectionMatrix());
}
