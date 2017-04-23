/*!
 *@brief	シェーダーのチュートリアル00
 */
#include "stdafx.h"
#include "Camera.h"
#include "Model.h"

//-----------------------------------------------------------------------------
// グローバル変数。
//-----------------------------------------------------------------------------

const int				LIGHT_NUM = 4;
D3DXVECTOR4 			g_diffuseLightDirection[LIGHT_NUM];	//ライトの方向。
D3DXVECTOR4				g_diffuseLightColor[LIGHT_NUM];		//ライトの色。
D3DXVECTOR4				g_ambientLight;						//環境光

Camera camera;				//カメラ。

Model originalModel;		//オリジナルモデル。
Model morphTargetA;			//モーフターゲットＡ。
Model morphTargetB;			//モーフターゲットＢ。
float morphRate = 0.0f;		//モーフレート。
/*!-----------------------------------------------------------------------------
 *@brief	ライトを更新。
 -----------------------------------------------------------------------------*/
void UpdateLight()
{
	//環境光。
	g_ambientLight = D3DXVECTOR4(0.8f, 0.8f, 0.8f, 1.0f);
}
//-----------------------------------------------------------------------------
// Name: ゲームを初期化。
//-----------------------------------------------------------------------------
void Init()
{
	//ライトを初期化。
	ZeroMemory(g_diffuseLightDirection, sizeof(g_diffuseLightDirection));
	ZeroMemory(g_diffuseLightColor, sizeof(g_diffuseLightColor));

	//カメラの初期化。
	camera.Init();

	originalModel.Init(g_pd3dDevice, "org.X");			//オリジナルモデルをロード。
	morphTargetA.Init(g_pd3dDevice, "morphTargetA.X");	//モーフターゲットAをロード。
	morphTargetB.Init(g_pd3dDevice, "morphTargetB.X");	//モーフターゲットBをロード。
}
//-----------------------------------------------------------------------------
// Name: 描画処理。
//-----------------------------------------------------------------------------
VOID Render()
{
	// 画面をクリア。
	g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 255), 1.0f, 0);
	//シーンの描画開始。
	g_pd3dDevice->BeginScene();
	D3DXMATRIX mWorld;
	D3DXMatrixIdentity(&mWorld);
	//ユニティちゃんを描画。
	originalModel.Render(
		g_pd3dDevice,
		mWorld,
		mWorld,
		camera.GetViewMatrix(),
		camera.GetProjectionMatrix(),
		g_diffuseLightDirection,
		g_diffuseLightColor,
		g_ambientLight,
		LIGHT_NUM
	);
	// シーンの描画終了。
	g_pd3dDevice->EndScene();
	// バックバッファとフロントバッファを入れ替える。
	g_pd3dDevice->Present(NULL, NULL, NULL, NULL);
}
/*!-----------------------------------------------------------------------------
 *@brief	更新処理。
 -----------------------------------------------------------------------------*/
void Update()
{
	if (GetAsyncKeyState(VK_LEFT)) {
		morphRate = max(0.0f, morphRate - 0.03f);
	}
	if (GetAsyncKeyState(VK_RIGHT)) {
		morphRate = min(1.0f, morphRate + 0.03f);
	}
	originalModel.Morphing(&morphTargetA, &morphTargetB, morphRate);
	//ライトの更新。
	UpdateLight();
	//カメラの更新
	camera.Update();
}
//-----------------------------------------------------------------------------
//ゲームが終了するときに呼ばれる処理。
//-----------------------------------------------------------------------------
void Terminate()
{
}
