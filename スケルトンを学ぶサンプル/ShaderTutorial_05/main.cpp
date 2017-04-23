/*!
 *@brief	シェーダーのチュートリアル00
 */
#include "stdafx.h"
#include "Camera.h"
#include "Ball.h"
#include "Skelton.h"

//-----------------------------------------------------------------------------
// グローバル変数。
//-----------------------------------------------------------------------------

const int				LIGHT_NUM = 4;
D3DXVECTOR4 			g_diffuseLightDirection[LIGHT_NUM];	//ライトの方向。
D3DXVECTOR4				g_diffuseLightColor[LIGHT_NUM];		//ライトの色。
D3DXVECTOR4				g_ambientLight;						//環境光

Camera camera;				//カメラ。

const int NUM_BALL = 7;
Ball ball[NUM_BALL];	//ボール。
Skelton ballSkelton;	//ボールのスケルトン。

enum EJoint {
	eJointPelvis,
	eJointLeftFoot ,
	eJointRightFoot ,
	eJointBody,
	eJointHead,
	eJointLeftHand,
	eJointRightHand,
};
/*!-----------------------------------------------------------------------------
 *@brief	ライトを更新。
 -----------------------------------------------------------------------------*/
void UpdateLight()
{
	g_diffuseLightDirection[0] = D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f);

	//ディフューズライト。
	g_diffuseLightColor[0] = D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f);

//環境光。
g_ambientLight = D3DXVECTOR4(0.2f, 0.2f, 0.2f, 1.0f);
}
//-----------------------------------------------------------------------------
// Name: ゲームを初期化。
//-----------------------------------------------------------------------------
void Init()
{
	//ライトを初期化。
	ZeroMemory(g_diffuseLightDirection, sizeof(g_diffuseLightDirection));
	ZeroMemory(g_diffuseLightColor, sizeof(g_diffuseLightColor));


	//スケルトンを構築。
	//骨盤　これがルート
	Joint joint;
	D3DXQuaternionIdentity(&joint.rotation);
	joint.position.x = 0.0f;
	joint.position.y = 0.0f;
	joint.position.z = 0.0f;
	ballSkelton.AddJoint(-1, &joint);
	//左足
	joint.position.x = -0.5f;
	joint.position.y = -0.5f;
	joint.position.z = 0.0f;
	ballSkelton.AddJoint(0, &joint);
	//右足。
	joint.position.x = 0.5f;
	joint.position.y = -0.5f;
	joint.position.z = 0.0f;
	ballSkelton.AddJoint(0, &joint);
	//胴体
	joint.position.x = 0.0f;
	joint.position.y = 0.5f;
	joint.position.z = 0.0f;
	ballSkelton.AddJoint(0, &joint);
	//頭
	joint.position.x = 0.0f;
	joint.position.y = 1.0f;
	joint.position.z = 0.0f;
	ballSkelton.AddJoint(3, &joint);
	//左手
	joint.position.x = -0.5f;
	joint.position.y = 0.5f;
	joint.position.z = 0.0f;
	ballSkelton.AddJoint(3, &joint);
	//右足
	joint.position.x = 0.5f;
	joint.position.y = 0.5f;
	joint.position.z = 0.0f;
	ballSkelton.AddJoint(3, &joint);


	//ボールを初期化。
	for (int i = 0; i < NUM_BALL; i++) {
		ball[i].Init(&ballSkelton, i);
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
	for (int i = 0; i < NUM_BALL; i++) {
		ball[i].Render(
			g_pd3dDevice,
			camera.GetViewMatrix(),
			camera.GetProjectionMatrix(),
			g_diffuseLightDirection,
			g_diffuseLightColor,
			g_ambientLight,
			LIGHT_NUM
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
	UpdateLight();
	//カメラの更新
	camera.Update();
	//グローバルポーズの更新
	ballSkelton.CalcGlobalPose();
	//骨盤を動かす
	if (GetAsyncKeyState(VK_UP)) {
		Joint* joint = ballSkelton.GetJoint(eJointPelvis);
		joint->localPosition.y += 0.01f;
	}
	//胴体を動かす
	if (GetAsyncKeyState('W')){
		Joint* joint = ballSkelton.GetJoint(eJointBody);
		joint->localPosition.y += 0.01f;
	}
	if (GetAsyncKeyState('A')) {
		Joint* joint = ballSkelton.GetJoint(eJointBody);
		static float angle = 0.0f;
		D3DXQuaternionRotationAxis(&joint->localRotation, &D3DXVECTOR3(0.0f, 1.0f, 0.0f), angle);
		angle+=0.01f;
	}

}
//-----------------------------------------------------------------------------
//ゲームが終了するときに呼ばれる処理。
//-----------------------------------------------------------------------------
void Terminate()
{
}
