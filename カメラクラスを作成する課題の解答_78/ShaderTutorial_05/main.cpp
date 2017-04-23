/*!
 *@brief	シェーダーのチュートリアル00
 */
#include <d3d9.h>
#include <d3dx9effect.h>
#pragma warning( disable : 4996 ) 
#include <strsafe.h>
#pragma warning( default : 4996 )
#include <cstdlib>


//-----------------------------------------------------------------------------
// グローバル変数。
//-----------------------------------------------------------------------------
LPDIRECT3D9             g_pD3D = NULL;		
LPDIRECT3DDEVICE9       g_pd3dDevice = NULL;
ID3DXEffect*			g_pEffect = NULL;	

D3DXMATRIX				g_worldMatrix;		//ワールド行列。モデルローカル空間から、ワールド空間に変換する行列。
D3DXMATRIX				g_rotationMatrix;	//回転行列。法線を回すために必要なので別途用意。

LPD3DXMESH				g_pMesh = NULL;
LPDIRECT3DTEXTURE9*	 	g_pMeshTextures = NULL; 	// Textures for our mesh
DWORD              	 	g_dwNumMaterials = 0L;   	// Number of mesh materials


static const int		LIGHT_NUM = 4;
D3DXVECTOR4 			g_diffuseLightDirection[LIGHT_NUM];	//ライトの方向。
D3DXVECTOR4				g_diffuseLightColor[LIGHT_NUM];		//ライトの色。
D3DXVECTOR4				g_ambientLight;						//環境光

//@カメラクラスを作るためのヒント。これらはカメラクラスのメンバ変数に変更する必要がある。
D3DXMATRIX				g_viewMatrix;		//ビュー行列。カメラ行列とも言う。
D3DXMATRIX				g_projectionMatrix;	//プロジェクション行列。ビュー空間から射影空間に変換する行列。


/*!
 *@brief	シェーダーエフェクトファイル(*.fx)をロード。
 */
void LoadEffectFile()
{
	LPD3DXBUFFER  compileErrorBuffer = NULL;
	//シェーダーをコンパイル。
	HRESULT hr = D3DXCreateEffectFromFile(
		g_pd3dDevice,
		"basic.fx",
		NULL,
		NULL,
#ifdef _DEBUG
		D3DXSHADER_DEBUG,
#else
		D3DXSHADER_SKIPVALIDATION,
#endif
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
HRESULT InitD3D(HWND hWnd)
{
	// Create the D3D object.
	if (NULL == (g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)))
		return E_FAIL;

	// Set up the structure used to create the D3DDevice
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	// Create the D3DDevice
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp, &g_pd3dDevice)))
	{
		return E_FAIL;
	}

	// Device state would normally be set here
	//シェーダーエフェクトファイルのロード。
	LoadEffectFile();
	return S_OK;
}

/*!
 *@brief	ビュープロジェクション行列の初期化。
 * @カメラクラスを作るためのヒント。カメラのクラスのメンバ変数にする必要がある。
 */
void InitViewProjectionMatrix()
{
	D3DXVECTOR3 vEyePt(0.0f, 3.0f, -5.0f);			//カメラクラスを作るためのヒント。カメラの視点。この辺りはメンバ変数に変更する。
	D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);		//カメラクラスを作るためのヒント。カメラの注視点。どこを見ているかという情報。この辺りはメンバ変数に変更する。
	D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);			//カメラクラスを作るためのヒント。カメラの上方向。基本的にY方向でかまわない。基本的には・・・。この辺りはメンバ変数に変更する。
	//カメラのワールド行列と回転行列を求める。カメラの回転*平行移動。
	D3DXMATRIX mCameraWorld, mCameraRotation;
	D3DXMatrixIdentity(&mCameraWorld);		//単位行列で初期化。
	D3DXMatrixIdentity(&mCameraRotation);
	//まず、カメラ座標系の基底軸を求める。
	//まずZの基底軸はカメラの方向なので、注視点から視点を減算して正規化する。
	D3DXVECTOR3 zAxis = vLookatPt - vEyePt;
	D3DXVec3Normalize(&zAxis, &zAxis);		//これがカメラ座標系のZ方向の基底軸。
	//続いて、Xの基底軸を求める。
	//Xの基底軸はZの基底軸と上方向に直行するベクトルになるため、の外積を計算すれば求まる。
	D3DXVECTOR3 xAxis;
	D3DXVec3Cross(&xAxis, &vUpVec, &zAxis);
	D3DXVec3Normalize(&xAxis, &xAxis);		//正規化。
	//最後にY軸の基底軸を求める。
	//この基底軸はZ軸とX軸の直行したベクトルになるため、外積を計算して求める。
	D3DXVECTOR3 yAxis;
	D3DXVec3Cross(&yAxis, &zAxis, &xAxis);
	D3DXVec3Normalize(&yAxis, &yAxis);
	//回転行列というのは、その座標系の基底軸になる。
	//そのためカメラの回転行列はここまでで求めた基底軸になる。
	mCameraWorld.m[0][0] = xAxis.x;
	mCameraWorld.m[0][1] = xAxis.y;
	mCameraWorld.m[0][2] = xAxis.z;
	mCameraWorld.m[0][3] = 0.0f;
	mCameraWorld.m[1][0] = yAxis.x;
	mCameraWorld.m[1][1] = yAxis.y;
	mCameraWorld.m[1][2] = yAxis.z;
	mCameraWorld.m[1][3] = 0.0f;
	mCameraWorld.m[2][0] = zAxis.x;
	mCameraWorld.m[2][1] = zAxis.y;
	mCameraWorld.m[2][2] = zAxis.z;
	mCameraWorld.m[2][3] = 0.0f;
	//カメラの回転行列を保存。
	mCameraRotation = mCameraWorld;
	//カメラの平行移動をワールド行列に直接反映させる。
	mCameraWorld.m[3][0] = vEyePt.x;
	mCameraWorld.m[3][1] = vEyePt.y;
	mCameraWorld.m[3][2] = vEyePt.z;
	mCameraWorld.m[3][3] = 1.0f;

	/*
	カメラ行列は3Dモデルをカメラから見た位置(カメラからの相対座標)に移動させるために使用されている。
	カメラから見た位置に移動させるということは、3Dモデルをカメラ座標系に変換するという意味である。
	そしてカメラ座標系に変換させるということは、カメラのワールド行列の逆行列を使用することを意味する。
	
	では、実際にサンプルコードを見ながら逆行列の使い方について学んでみましょう。
	例えば、あるオブジェクトをプレイヤー座標系に変換したいのであれば、そのオブジェクトの座標に
	プレイヤーのワールド行列の逆行列を乗算してやればよい。
	下記にサンプルコードを記述する。
	
	サンプルコード。
	 void CPlayer::Update()
	 {
			//プレイヤーのワールド行列を更新。
			D3DXMATRIX mTrans, mRot;
			D3DXMatrixTranslation(&mTrans, &m_position);		//平行移動行列を作成。
			D3DXMatrixRotationQuaternion(&mRot, &m_rotation);	//回転行列を作成。
			m_worldMatrix = mRot * mTrans;						//ワールド行列を計算。
			//プレイヤーのワールド行列の逆行列を計算する。
			D3DXMATRIX mWorldInv;
			D3DXMatrixInverse(&mWorldInv, NULL, &m_worldMatrix);
			//ボス敵の座標をプレイヤーから見た座標に変換する。
			D3DXVECTOR3 bossPosInPlayer = g_boss.GetPosition();
			D3DXVec3Transform(&bossPosInPlayer, &bossPosInPlayer, &mWorldInv);
	 }
	
	つまり、カメラ座標系に変換するということはカメラのワールド行列の逆行列を乗算することを意味している。
	よって、カメラ行列 = カメラのワールド行列の逆行列ということになる。
	*/

	//逆行列の計算
	D3DXMatrixInverse(&g_viewMatrix, NULL, &mCameraWorld);

	D3DXMATRIXA16 matProj;
    D3DXMatrixPerspectiveFovLH( &g_projectionMatrix, D3DX_PI / 4, 1.0f, 1.0f, 100.0f );
}


//-----------------------------------------------------------------------------
// Name: Cleanup()
// Desc: Releases all previously initialized objects
//-----------------------------------------------------------------------------
VOID Cleanup()
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
	if (g_pd3dDevice != NULL)
		g_pd3dDevice->Release();

	if (g_pD3D != NULL)
		g_pD3D->Release();
}
/*!
 *@brief	ライトを更新。
 */
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
	g_ambientLight = D3DXVECTOR4(0.1f, 0.1f, 0.1f, 1.0f);
}
//-----------------------------------------------------------------------------
// Name: Render()
// Desc: Draws the scene
//-----------------------------------------------------------------------------
VOID Render()
{
	// Clear the backbuffer to a blue color
	g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 255), 1.0f, 0);
	
	static int renderCount = 0;
	
	if (SUCCEEDED(g_pd3dDevice->BeginScene()))
	{
		renderCount++;
		D3DXMATRIXA16 matWorld;
    	D3DXMatrixRotationY( &g_worldMatrix, renderCount / 500.0f ); 
    	g_rotationMatrix = g_worldMatrix;
		//ライトを更新
		UpdateLight();
		// Turn on the zbuffer
		g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
		
		//シェーダー適用開始。
		g_pEffect->SetTechnique("SkinModel");
		g_pEffect->Begin(NULL, D3DXFX_DONOTSAVESHADERSTATE);
		g_pEffect->BeginPass(0);

		//定数レジスタに設定するカラー。
		D3DXVECTOR4 color( 1.0f, 0.0f, 0.0f, 1.0f);
		//ワールド行列の転送。
		g_pEffect->SetMatrix("g_worldMatrix", &g_worldMatrix);
		//ビュー行列の転送。
		g_pEffect->SetMatrix("g_viewMatrix", &g_viewMatrix);				//@カメラクラスを作るためのヒント。 カメラクラスのゲッターを使用するようにする。
		//プロジェクション行列の転送。
		g_pEffect->SetMatrix("g_projectionMatrix", &g_projectionMatrix);	//@カメラクラスを作るためのヒント。 カメラクラスのゲッターを使用するようにする。
		//回転行列を転送。
		g_pEffect->SetMatrix( "g_rotationMatrix", &g_rotationMatrix );
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

/*!
 *@brief	Xファイルを読み込んでジオメトリを初期化。
 */
HRESULT InitGeometry()
{
    LPD3DXBUFFER pD3DXMtrlBuffer;

    // Load the mesh from the specified file
    if( FAILED( D3DXLoadMeshFromX( "Tiger.x", D3DXMESH_SYSTEMMEM,
                                   g_pd3dDevice, NULL,
                                   &pD3DXMtrlBuffer, NULL, &g_dwNumMaterials,
                                   &g_pMesh ) ) )
    {
        // If model is not in current folder, try parent folder
        if( FAILED( D3DXLoadMeshFromX( "..\\Tiger.x", D3DXMESH_SYSTEMMEM,
                                       g_pd3dDevice, NULL,
                                       &pD3DXMtrlBuffer, NULL, &g_dwNumMaterials,
                                       &g_pMesh ) ) )
        {
            MessageBox( NULL, "Could not find tiger.x", "Meshes.exe", MB_OK );
            return E_FAIL;
        }
    }
	//法線が存在するか調べる。
	if ((g_pMesh->GetFVF() & D3DFVF_NORMAL) == 0) {
		//法線がないので作成する。
		ID3DXMesh* pTempMesh = NULL;

		g_pMesh->CloneMeshFVF(g_pMesh->GetOptions(),
			g_pMesh->GetFVF() | D3DFVF_NORMAL, g_pd3dDevice, &pTempMesh);

		D3DXComputeNormals(pTempMesh, NULL);
		g_pMesh->Release();
		g_pMesh = pTempMesh;

	}
    // We need to extract the material properties and texture names from the 
    // pD3DXMtrlBuffer
    D3DXMATERIAL* d3dxMaterials = ( D3DXMATERIAL* )pD3DXMtrlBuffer->GetBufferPointer();
   
    g_pMeshTextures = new LPDIRECT3DTEXTURE9[g_dwNumMaterials];
    if( g_pMeshTextures == NULL )
        return E_OUTOFMEMORY;

    for( DWORD i = 0; i < g_dwNumMaterials; i++ )
    {
        g_pMeshTextures[i] = NULL;
        if( d3dxMaterials[i].pTextureFilename != NULL &&
            lstrlenA( d3dxMaterials[i].pTextureFilename ) > 0 )
        {
            // Create the texture
            if( FAILED( D3DXCreateTextureFromFileA( g_pd3dDevice,
                                                    d3dxMaterials[i].pTextureFilename,
                                                    &g_pMeshTextures[i] ) ) )
            {
                // If texture is not in current folder, try parent folder
                const CHAR* strPrefix = "..\\";
                CHAR strTexture[MAX_PATH];
                strcpy_s( strTexture, MAX_PATH, strPrefix );
                strcat_s( strTexture, MAX_PATH, d3dxMaterials[i].pTextureFilename );
                // If texture is not in current folder, try parent folder
                if( FAILED( D3DXCreateTextureFromFileA( g_pd3dDevice,
                                                        strTexture,
                                                        &g_pMeshTextures[i] ) ) )
                {
                    MessageBox( NULL, "Could not find texture map", "Meshes.exe", MB_OK );
                }
            }
        }
    }

    // Done with the material buffer
    pD3DXMtrlBuffer->Release();

    return S_OK;
}

//-----------------------------------------------------------------------------
// Name: wWinMain()
// Desc: The application's entry point
//-----------------------------------------------------------------------------
INT WINAPI wWinMain(HINSTANCE hInst, HINSTANCE, LPWSTR, INT)
{
	UNREFERENCED_PARAMETER(hInst);

	// Register the window class
	WNDCLASSEX wc =
	{
		sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L,
		GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
		"Shader Tutorial", NULL
	};
	RegisterClassEx(&wc);

	// Create the application's window
	HWND hWnd = CreateWindow("Shader Tutorial", "Shader Tutorial 00",
		WS_OVERLAPPEDWINDOW, 100, 100, 500, 500,
		NULL, NULL, wc.hInstance, NULL);

	// Initialize Direct3D
	if (SUCCEEDED(InitD3D(hWnd)))
	{
		// Create the vertex buffer
		if (SUCCEEDED(InitGeometry()))
		{
			ZeroMemory( g_diffuseLightDirection, sizeof(g_diffuseLightDirection) );
			ZeroMemory( g_diffuseLightColor, sizeof(g_diffuseLightColor) );
			
			D3DXMatrixIdentity(&g_worldMatrix);
			D3DXMatrixIdentity(&g_rotationMatrix);


			InitViewProjectionMatrix();
			// Show the window
			ShowWindow(hWnd, SW_SHOWDEFAULT);
			UpdateWindow(hWnd);

			// Enter the message loop
			MSG msg;
			ZeroMemory(&msg, sizeof(msg));
			while (msg.message != WM_QUIT)
			{
				if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
				else
					Render();
			}
		}
	}

	UnregisterClass("Shader Tutorial", wc.hInstance);
	return 0;
}
