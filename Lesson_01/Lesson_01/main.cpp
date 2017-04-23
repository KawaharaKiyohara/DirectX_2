/*!
 *@brief	DirectXⅡ Lesson_01
 */
#include "stdafx.h"


//-----------------------------------------------------------------------------
// グローバル変数。
//-----------------------------------------------------------------------------
////////////////////////////////////////////////
// DirectXのシステム関係の変数
////////////////////////////////////////////////
LPDIRECT3D9             g_pD3D = NULL;		
LPDIRECT3DDEVICE9       g_pd3dDevice = NULL;

////////////////////////////////////////////////
// モデル表示に関係する変数。
////////////////////////////////////////////////
ID3DXEffect*			g_pEffect = NULL;	
D3DXMATRIX				g_worldMatrix;				//ワールド行列。モデルローカル空間から、ワールド空間に変換する行列。
LPD3DXMESH				g_pMesh = NULL;
LPDIRECT3DTEXTURE9*	 	g_pMeshTextures = NULL; 	// テクスチャ。
DWORD              	 	g_dwNumMaterials = 0L;   	// マテリアルの数。

////////////////////////////////////////////////
// ライトに関係する変数。
////////////////////////////////////////////////
D3DXVECTOR4 			g_diffuseLightDirection[LIGHT_NUM];	//ライトの方向。
D3DXVECTOR4				g_diffuseLightColor[LIGHT_NUM];		//ライトの色。
D3DXVECTOR4				g_ambientLight;						//環境光

////////////////////////////////////////////////
// カメラに関係する変数。
////////////////////////////////////////////////
D3DXMATRIX				g_viewMatrix;						//ビュー行列。
D3DXMATRIX				g_projectionMatrix;					//プロジェクション行列。
void RenderTiger();
void CleanupTiger();
/*!-----------------------------------------------------------------------------
 *@brief	シェーダーエフェクトファイル(*.fx)をロード。
 -----------------------------------------------------------------------------*/
void LoadEffectFile()
{
	LPD3DXBUFFER  compileErrorBuffer = NULL;
	//シェーダーをコンパイル。
	HRESULT hr = D3DXCreateEffectFromFile(
		g_pd3dDevice,
		"basic.fx",
		NULL,
		NULL,
		D3DXSHADER_SKIPVALIDATION,
		NULL,
		&g_pEffect,
		&compileErrorBuffer
		);
	if (FAILED(hr)) {
		MessageBox(NULL, (char*)(compileErrorBuffer->GetBufferPointer()), "error", MB_OK);
		std::abort();
	}
}
//-----------------------------------------------------------------------------
// Name: InitD3D()
// Desc: Initializes Direct3D
//-----------------------------------------------------------------------------
void InitD3D(HWND hWnd)
{
	//D3Dオブジェクトを作成する。
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	
	// D3Dデバイスを作成するためのパラメータを設定する。
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	//D3Dデバイスを作成する。
	g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp, &g_pd3dDevice);

}

//-----------------------------------------------------------------------------
// Name: Cleanup()
// Desc: Releases all previously initialized objects
//-----------------------------------------------------------------------------
void Cleanup()
{
	CleanupTiger();
	if (g_pd3dDevice != NULL)
		g_pd3dDevice->Release();

	if (g_pD3D != NULL)
		g_pD3D->Release();
}
/*!-----------------------------------------------------------------------------
 *@brief	ライトを更新。
 -----------------------------------------------------------------------------*/
void UpdateLight()
{
	g_diffuseLightDirection[0] = D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f),
	g_diffuseLightDirection[1] = D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f),
	g_diffuseLightDirection[2] = D3DXVECTOR4(0.0f, -1.0f, 0.0f, 1.0f),
	g_diffuseLightDirection[3] = D3DXVECTOR4(-1.0f, 0.0f, 0.0f, 1.0f),
	
	//ディフューズライト。
	g_diffuseLightColor[0] = D3DXVECTOR4(0.5f, 0.5f, 0.5f, 1.0f);
	g_diffuseLightColor[1] = D3DXVECTOR4(0.5f, 0.5f, 0.5f, 1.0f);
	g_diffuseLightColor[2] = D3DXVECTOR4(0.5f, 0.4f, 0.6f, 1.0f);
	g_diffuseLightColor[3] = D3DXVECTOR4(0.2f, 0.2f, 0.2f, 1.0f);

	//環境光。
	g_ambientLight = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
}

//-----------------------------------------------------------------------------
// Name: シーンを描画処理。
//-----------------------------------------------------------------------------
void Render()
{
	// 画面をクリア。
	g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 255), 1.0f, 0);
	
	if (SUCCEEDED(g_pd3dDevice->BeginScene()))
	{
		//ライトを更新
		UpdateLight();
		// Turn on the zbuffer
		g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
		
		//虎を描画。
		RenderTiger();

		// End the scene
		g_pd3dDevice->EndScene();
	}

	// Present the backbuffer contents to the display
	g_pd3dDevice->Present(NULL, NULL, NULL, NULL);
}




//-----------------------------------------------------------------------------
// Name: MsgProc()
// Desc: The window's message handler
//-----------------------------------------------------------------------------
LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		Cleanup();
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}
/*!-----------------------------------------------------------------------------
*@brief	カメラの初期化。
-----------------------------------------------------------------------------*/
void InitCamera()
{
	D3DXVECTOR3 vEyePt = D3DXVECTOR3(0.0f, 3.0f, -5.0f);
	D3DXVECTOR3 vLookatPt = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vUpVec = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&g_viewMatrix, &vEyePt, &vLookatPt, &vUpVec);
	D3DXMatrixPerspectiveFovLH(&g_projectionMatrix, D3DX_PI / 4, 1.0f, 0.1f, 10000.0f);
}
/*!-----------------------------------------------------------------------------
 *@brief	虎のXファイルを読み込んでジオメトリを初期化。
 -----------------------------------------------------------------------------*/
void InitTigerGeometry()
{
	//ワールド行列を単位行列に初期化する。
	D3DXMatrixIdentity(&g_worldMatrix);
	
    LPD3DXBUFFER pD3DXMtrlBuffer;
    //Xファイルのロード。
	D3DXLoadMeshFromX( "Tiger.x", D3DXMESH_SYSTEMMEM,
                                   g_pd3dDevice, NULL,
                                   &pD3DXMtrlBuffer, NULL, &g_dwNumMaterials,
                                   &g_pMesh );
    // マテリアルバッファを取得。
    D3DXMATERIAL* d3dxMaterials = ( D3DXMATERIAL* )pD3DXMtrlBuffer->GetBufferPointer();
   
    g_pMeshTextures = new LPDIRECT3DTEXTURE9[g_dwNumMaterials];
   
    for( DWORD i = 0; i < g_dwNumMaterials; i++ )
    {
        g_pMeshTextures[i] = NULL;
        //テクスチャを作成する。
		D3DXCreateTextureFromFileA( g_pd3dDevice,
                                    d3dxMaterials[i].pTextureFilename,
                                    &g_pMeshTextures[i] );
    }
    // マテリアルバッファを解放。
    pD3DXMtrlBuffer->Release();
}
//-----------------------------------------------------------------------------
// Name: 虎を描画。
//-----------------------------------------------------------------------------
void RenderTiger()
{
	//シェーダー適用開始。
	g_pEffect->SetTechnique("SkinModel");
	g_pEffect->Begin(NULL, D3DXFX_DONOTSAVESHADERSTATE);
	g_pEffect->BeginPass(0);

	//定数レジスタに設定するカラー。
	D3DXVECTOR4 color( 1.0f, 0.0f, 0.0f, 1.0f);
	//ワールド行列の転送。
	g_pEffect->SetMatrix("g_worldMatrix", &g_worldMatrix);
	//ビュー行列の転送。
	g_pEffect->SetMatrix("g_viewMatrix", &g_viewMatrix);
	//プロジェクション行列の転送。
	g_pEffect->SetMatrix("g_projectionMatrix", &g_projectionMatrix);
	//ライトの向きを転送。
	g_pEffect->SetVectorArray("g_diffuseLightDirection", g_diffuseLightDirection, LIGHT_NUM );
	//ライトのカラーを転送。
	g_pEffect->SetVectorArray("g_diffuseLightColor", g_diffuseLightColor, LIGHT_NUM );
	//環境光を設定。
	g_pEffect->SetVector("g_ambientLight", &g_ambientLight);

	g_pEffect->CommitChanges();						//この関数を呼び出すことで、データの転送が確定する。描画を行う前に一回だけ呼び出す。
	
	// Meshes are divided into subsets, one for each material. Render them in
    // a loop
    for( DWORD i = 0; i < g_dwNumMaterials; i++ )
    {
		g_pEffect->SetTexture("g_diffuseTexture", g_pMeshTextures[i]);
        // Draw the mesh subset
        g_pMesh->DrawSubset( i );
    }
    
	g_pEffect->EndPass();
	g_pEffect->End();
}
//-----------------------------------------------------------------------------
// 虎に関係するオブジェクトのクリーンアップ。
//-----------------------------------------------------------------------------
void CleanupTiger()
{
	if (g_pMeshTextures != NULL) {
		for (int i = 0; i < g_dwNumMaterials; i++) {
			g_pMeshTextures[i]->Release();
		}
		delete[] g_pMeshTextures;
	}
	if (g_pMesh != NULL) {
		g_pMesh->Release();
	}
	if (g_pEffect != NULL) {
		g_pEffect->Release();
	}
}
/*!-----------------------------------------------------------------------------
 *@brief	更新処理。
 -----------------------------------------------------------------------------*/
void Update()
{
}
//-----------------------------------------------------------------------------
// エントリー関数。
//-----------------------------------------------------------------------------
INT WINAPI wWinMain(HINSTANCE hInst, HINSTANCE, LPWSTR, INT)
{
	UNREFERENCED_PARAMETER(hInst);

	// Register the window class
	WNDCLASSEX wc =
	{
		sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L,
		GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
		"Lesson 01", NULL
	};
	RegisterClassEx(&wc);

	// Create the application's window
	HWND hWnd = CreateWindow("Lesson 01", "Lesson 01",
		WS_OVERLAPPEDWINDOW, 100, 100, 500, 500,
		NULL, NULL, wc.hInstance, NULL);

	// Direct3Dを初期化。
	InitD3D(hWnd);
	//虎のモデルを初期化。
	InitTigerGeometry();
	//エフェクトファイルのロード。
	LoadEffectFile();
	
	//ライトの初期化。
	ZeroMemory( g_diffuseLightDirection, sizeof(g_diffuseLightDirection) );
	ZeroMemory( g_diffuseLightColor, sizeof(g_diffuseLightColor) );

	//カメラの初期化。
	InitCamera();
	
	// Show the window
	ShowWindow(hWnd, SW_SHOWDEFAULT);
	UpdateWindow(hWnd);

	// ゲームループ
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			Update();
			Render();
		}
	}
	

	UnregisterClass("Lesson 01", wc.hInstance);
	return 0;
}
