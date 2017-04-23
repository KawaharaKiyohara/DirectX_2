#include "stdafx.h"
#include "Tiger.h"

//コンストラクタ
Tiger::Tiger()
{
	//初期化。
	
	D3DXMatrixIdentity(&mWorld);
	D3DXMatrixIdentity(&mRotation);
	position.x = 0.0f;
	position.y = 0.0f;
	position.z = 0.0f;
}
//デストラクタ
Tiger::~Tiger()
{
}
//座標を設定。
void Tiger::SetPosition(D3DXVECTOR3 pos)
{
	position = pos;
}
//初期化。
void Tiger::Init(LPDIRECT3DDEVICE9 pd3dDevice)
{
	model.Init(pd3dDevice, "tiger.x");
}
//更新。
void Tiger::Update()
{
	//ワールド行列の更新。
	D3DXMatrixTranslation(&mWorld, position.x, position.y, position.z);
}
//描画。
void Tiger::Render( 
	LPDIRECT3DDEVICE9 pd3dDevice, 
	D3DXMATRIX viewMatrix, 
	D3DXMATRIX projMatrix,
	D3DXVECTOR4* diffuseLightDirection,
	D3DXVECTOR4* diffuseLightColor,
	D3DXVECTOR4	 ambientLight,
	int numDiffuseLight
)
{
	model.Render(
		pd3dDevice,
		mWorld,
		mRotation,
		viewMatrix,
		projMatrix,
		diffuseLightDirection,
		diffuseLightColor,
		ambientLight,
		numDiffuseLight
	);
}
//開放。
void Tiger::Release()
{
	model.Release();
}