/*!
 *@brief	シェーダーのチュートリアル00
 */
#include "stdafx.h"
#include "Camera.h"
#include "Tiger.h"
#include "Light.h"
//-----------------------------------------------------------------------------
// ライト関係の変数。
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// それ以外のグローバル変数。
//-----------------------------------------------------------------------------
const int NUM_TIGER = 1;	//トラの数。
Camera camera;				//カメラ。
Tiger tiger[NUM_TIGER];		//虎。
Light light;				//ライトクラスのインスタンス。



//-----------------------------------------------------------------------------
// Name: ゲームを初期化。
//-----------------------------------------------------------------------------
void Init()
{
	
	//虎を初期化。
	for (int i = 0; i < NUM_TIGER; i++) {
		tiger[i].Init(g_pd3dDevice);
		int y = i / 10;
		int x = i % 10;
		tiger[i].SetPosition(D3DXVECTOR3(0.5f * x, 0.5f * y, 0.0f));
	}
	//カメラの初期化。
	camera.Init();

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
	//トラを描画。
	for (int i = 0; i < NUM_TIGER; i++) {
		tiger[i].Render(
			g_pd3dDevice,
			camera.GetViewMatrix(),
			camera.GetProjectionMatrix(),
			light.GetDiffuseLightDirection(),
			light.g_diffuseLightColor,
			light.g_ambientLight,
			light.LIGHT_NUM
			);
	}
	
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
	//ライトの更新。
	light.UpdateLight();
	//カメラの更新
	camera.Update();
	//虎の更新。
	for (int i = 0; i < NUM_TIGER; i++) {
		tiger[i].Update();
	}
}
//-----------------------------------------------------------------------------
//ゲームが終了するときに呼ばれる処理。
//-----------------------------------------------------------------------------
void Terminate()
{
	for (int i = 0; i < NUM_TIGER; i++) {
		tiger[i].Release();
	}
}
