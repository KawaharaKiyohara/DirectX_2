#include "stdafx.h"
#include "Player.h"
#include "game.h"
#include "lib/Input.h"

Player::Player()
{
	currentDirection = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
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
	light.SetAmbientLight(D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f));

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
	//パス移動を行う。
	//プレイヤーの座標をコース定義に射影していく。
	D3DXVECTOR3 courceDir = 
		g_courceDatas[currentCourceNodeNo].endPos 
		- g_courceDatas[currentCourceNodeNo].startPos;
	//コースの長さを求めてから、コースの向きを求める。
	float len = D3DXVec3Length(&courceDir);
	D3DXVec3Normalize(&courceDir, &courceDir);
	//プレイヤーがコース上のどの位置にいるかを調べる。
	D3DXVECTOR3 playerPosInCourceNode = 
		position 
		- g_courceDatas[currentCourceNodeNo].startPos;
	float t = D3DXVec3Dot(&playerPosInCourceNode, &courceDir);
	//進行方向はコースの向き。
	moveDir = courceDir;
	bool isPossibleMove = true;
	if (t > len) {
		//次のノードへ。
		currentCourceNodeNo++;
		if (currentCourceNodeNo >= COURCE_NODE_NUM) {
			//終端まで移動した。
			//移動はもうできない。
			currentCourceNodeNo = COURCE_NODE_NUM;
			isPossibleMove = false;
		}
	}
	//終了ノードに向かって移動する。
	if (isPossibleMove) {
		//移動出来たら。
		position += moveDir * moveSpeed;
	}
	//ワールド行列を更新。
	model.UpdateWorldMatrix(position, rotation, D3DXVECTOR3(1.0f, 1.0f, 1.0f));
}
void Player::Render()
{
	const Camera& camera = game->GetCamera();
	model.Draw(&camera.GetViewMatrix(), &camera.GetProjectionMatrix());
}