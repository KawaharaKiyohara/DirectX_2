#include "stdafx.h"
#include "GameCamera.h"
#include "Player.h"
#include "lib/Input.h"
#include "game.h"

GameCamera::GameCamera() :
	targetOffset(0.0f, 0.0f, 0.0f, 1.0f),
	cameraDir(0.0f, 0.0f, 0.0f)
{
}
GameCamera::~GameCamera()
{
}
void GameCamera::Start(Player* player)
{
	targetOffset.y = 1.0f;
	targetOffset.x = 0.0f;
	camera.SetAspect(1280.0f / 720.0f);
	camera.Init();
	this->player = player;
	UpdateCamera();

	target = player->GetPosition();
	eyePos = target;
	

}
//カメラを更新。

void GameCamera::UpdateCamera()
{
	//注視点はプレイヤーの座標。
	target = player->GetPosition();
	//現在のノードの向きを調べる。
	D3DXVECTOR3 courceDir
		= g_courceDatas[currentCourceNodeNo].endPos
		- g_courceDatas[currentCourceNodeNo].startPos;
	//現在のノードの長さを求める。
	float len = D3DXVec3Length(&courceDir);
	//ノードの向きを正規化する。
	D3DXVec3Normalize(&courceDir, &courceDir);

	//カメラがコース上のどの位置にいるかを調べる。
	D3DXVECTOR3 cameraPosInCourceNode =
		eyePos
		- g_courceDatas[currentCourceNodeNo].startPos;
	float t = D3DXVec3Dot(&cameraPosInCourceNode, &courceDir);

	//進行方向はコースの向き。
	isPossibleMove = true;
	if (t > len) {
		//次のノードへ。
		currentCourceNodeNo++;
		isPossibleMove = false;
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
		D3DXVECTOR3 toEyePos = courceDir * -10.0f;
		toEyePos.y = 2.0f;
		eyePos = target + toEyePos;
	}

	camera.SetEyePt(eyePos);
	camera.SetLookatPt(target);
	camera.Update();
}
void GameCamera::PreUpdate()
{
	/*cameraDir = D3DXVECTOR3(toEyePos);
	cameraDir.y = 0.0f;
	D3DXVec3Normalize(&cameraDir, &cameraDir);*/
}
void GameCamera::Update()
{
	UpdateCamera();
}