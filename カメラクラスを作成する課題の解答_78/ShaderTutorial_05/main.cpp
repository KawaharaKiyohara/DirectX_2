/*!
 *@brief	�V�F�[�_�[�̃`���[�g���A��00
 */
#include <d3d9.h>
#include <d3dx9effect.h>
#pragma warning( disable : 4996 ) 
#include <strsafe.h>
#pragma warning( default : 4996 )
#include <cstdlib>


//-----------------------------------------------------------------------------
// �O���[�o���ϐ��B
//-----------------------------------------------------------------------------
LPDIRECT3D9             g_pD3D = NULL;		
LPDIRECT3DDEVICE9       g_pd3dDevice = NULL;
ID3DXEffect*			g_pEffect = NULL;	

D3DXMATRIX				g_worldMatrix;		//���[���h�s��B���f�����[�J����Ԃ���A���[���h��Ԃɕϊ�����s��B
D3DXMATRIX				g_rotationMatrix;	//��]�s��B�@�����񂷂��߂ɕK�v�Ȃ̂ŕʓr�p�ӁB

LPD3DXMESH				g_pMesh = NULL;
LPDIRECT3DTEXTURE9*	 	g_pMeshTextures = NULL; 	// Textures for our mesh
DWORD              	 	g_dwNumMaterials = 0L;   	// Number of mesh materials


static const int		LIGHT_NUM = 4;
D3DXVECTOR4 			g_diffuseLightDirection[LIGHT_NUM];	//���C�g�̕����B
D3DXVECTOR4				g_diffuseLightColor[LIGHT_NUM];		//���C�g�̐F�B
D3DXVECTOR4				g_ambientLight;						//����

//@�J�����N���X����邽�߂̃q���g�B�����̓J�����N���X�̃����o�ϐ��ɕύX����K�v������B
D3DXMATRIX				g_viewMatrix;		//�r���[�s��B�J�����s��Ƃ������B
D3DXMATRIX				g_projectionMatrix;	//�v���W�F�N�V�����s��B�r���[��Ԃ���ˉe��Ԃɕϊ�����s��B


/*!
 *@brief	�V�F�[�_�[�G�t�F�N�g�t�@�C��(*.fx)�����[�h�B
 */
void LoadEffectFile()
{
	LPD3DXBUFFER  compileErrorBuffer = NULL;
	//�V�F�[�_�[���R���p�C���B
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
	//�V�F�[�_�[�G�t�F�N�g�t�@�C���̃��[�h�B
	LoadEffectFile();
	return S_OK;
}

/*!
 *@brief	�r���[�v���W�F�N�V�����s��̏������B
 * @�J�����N���X����邽�߂̃q���g�B�J�����̃N���X�̃����o�ϐ��ɂ���K�v������B
 */
void InitViewProjectionMatrix()
{
	D3DXVECTOR3 vEyePt(0.0f, 3.0f, -5.0f);			//�J�����N���X����邽�߂̃q���g�B�J�����̎��_�B���̕ӂ�̓����o�ϐ��ɕύX����B
	D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);		//�J�����N���X����邽�߂̃q���g�B�J�����̒����_�B�ǂ������Ă��邩�Ƃ������B���̕ӂ�̓����o�ϐ��ɕύX����B
	D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);			//�J�����N���X����邽�߂̃q���g�B�J�����̏�����B��{�I��Y�����ł��܂�Ȃ��B��{�I�ɂ́E�E�E�B���̕ӂ�̓����o�ϐ��ɕύX����B
	//�J�����̃��[���h�s��Ɖ�]�s������߂�B�J�����̉�]*���s�ړ��B
	D3DXMATRIX mCameraWorld, mCameraRotation;
	D3DXMatrixIdentity(&mCameraWorld);		//�P�ʍs��ŏ������B
	D3DXMatrixIdentity(&mCameraRotation);
	//�܂��A�J�������W�n�̊�ꎲ�����߂�B
	//�܂�Z�̊�ꎲ�̓J�����̕����Ȃ̂ŁA�����_���王�_�����Z���Đ��K������B
	D3DXVECTOR3 zAxis = vLookatPt - vEyePt;
	D3DXVec3Normalize(&zAxis, &zAxis);		//���ꂪ�J�������W�n��Z�����̊�ꎲ�B
	//�����āAX�̊�ꎲ�����߂�B
	//X�̊�ꎲ��Z�̊�ꎲ�Ə�����ɒ��s����x�N�g���ɂȂ邽�߁A�̊O�ς��v�Z����΋��܂�B
	D3DXVECTOR3 xAxis;
	D3DXVec3Cross(&xAxis, &vUpVec, &zAxis);
	D3DXVec3Normalize(&xAxis, &xAxis);		//���K���B
	//�Ō��Y���̊�ꎲ�����߂�B
	//���̊�ꎲ��Z����X���̒��s�����x�N�g���ɂȂ邽�߁A�O�ς��v�Z���ċ��߂�B
	D3DXVECTOR3 yAxis;
	D3DXVec3Cross(&yAxis, &zAxis, &xAxis);
	D3DXVec3Normalize(&yAxis, &yAxis);
	//��]�s��Ƃ����̂́A���̍��W�n�̊�ꎲ�ɂȂ�B
	//���̂��߃J�����̉�]�s��͂����܂łŋ��߂���ꎲ�ɂȂ�B
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
	//�J�����̉�]�s���ۑ��B
	mCameraRotation = mCameraWorld;
	//�J�����̕��s�ړ������[���h�s��ɒ��ڔ��f������B
	mCameraWorld.m[3][0] = vEyePt.x;
	mCameraWorld.m[3][1] = vEyePt.y;
	mCameraWorld.m[3][2] = vEyePt.z;
	mCameraWorld.m[3][3] = 1.0f;

	/*
	�J�����s���3D���f�����J�������猩���ʒu(�J��������̑��΍��W)�Ɉړ������邽�߂Ɏg�p����Ă���B
	�J�������猩���ʒu�Ɉړ�������Ƃ������Ƃ́A3D���f�����J�������W�n�ɕϊ�����Ƃ����Ӗ��ł���B
	�����ăJ�������W�n�ɕϊ�������Ƃ������Ƃ́A�J�����̃��[���h�s��̋t�s����g�p���邱�Ƃ��Ӗ�����B
	
	�ł́A���ۂɃT���v���R�[�h�����Ȃ���t�s��̎g�����ɂ��Ċw��ł݂܂��傤�B
	�Ⴆ�΁A����I�u�W�F�N�g���v���C���[���W�n�ɕϊ��������̂ł���΁A���̃I�u�W�F�N�g�̍��W��
	�v���C���[�̃��[���h�s��̋t�s�����Z���Ă��΂悢�B
	���L�ɃT���v���R�[�h���L�q����B
	
	�T���v���R�[�h�B
	 void CPlayer::Update()
	 {
			//�v���C���[�̃��[���h�s����X�V�B
			D3DXMATRIX mTrans, mRot;
			D3DXMatrixTranslation(&mTrans, &m_position);		//���s�ړ��s����쐬�B
			D3DXMatrixRotationQuaternion(&mRot, &m_rotation);	//��]�s����쐬�B
			m_worldMatrix = mRot * mTrans;						//���[���h�s����v�Z�B
			//�v���C���[�̃��[���h�s��̋t�s����v�Z����B
			D3DXMATRIX mWorldInv;
			D3DXMatrixInverse(&mWorldInv, NULL, &m_worldMatrix);
			//�{�X�G�̍��W���v���C���[���猩�����W�ɕϊ�����B
			D3DXVECTOR3 bossPosInPlayer = g_boss.GetPosition();
			D3DXVec3Transform(&bossPosInPlayer, &bossPosInPlayer, &mWorldInv);
	 }
	
	�܂�A�J�������W�n�ɕϊ�����Ƃ������Ƃ̓J�����̃��[���h�s��̋t�s�����Z���邱�Ƃ��Ӗ����Ă���B
	����āA�J�����s�� = �J�����̃��[���h�s��̋t�s��Ƃ������ƂɂȂ�B
	*/

	//�t�s��̌v�Z
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
 *@brief	���C�g���X�V�B
 */
void UpdateLight()
{
	g_diffuseLightDirection[0] = D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f),
	g_diffuseLightDirection[1] = D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f),
	g_diffuseLightDirection[2] = D3DXVECTOR4(0.0f, -1.0f, 0.0f, 1.0f),
	g_diffuseLightDirection[3] = D3DXVECTOR4(-1.0f, 0.0f, 0.0f, 1.0f),
	
	//�f�B�t���[�Y���C�g�B
	g_diffuseLightColor[0] = D3DXVECTOR4(0.5f, 0.5f, 0.5f, 1.0f);
	g_diffuseLightColor[1] = D3DXVECTOR4(0.5f, 0.5f, 0.5f, 1.0f);
	g_diffuseLightColor[2] = D3DXVECTOR4(0.5f, 0.4f, 0.6f, 1.0f);
	g_diffuseLightColor[3] = D3DXVECTOR4(0.2f, 0.2f, 0.2f, 1.0f);

	//�����B
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
		//���C�g���X�V
		UpdateLight();
		// Turn on the zbuffer
		g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
		
		//�V�F�[�_�[�K�p�J�n�B
		g_pEffect->SetTechnique("SkinModel");
		g_pEffect->Begin(NULL, D3DXFX_DONOTSAVESHADERSTATE);
		g_pEffect->BeginPass(0);

		//�萔���W�X�^�ɐݒ肷��J���[�B
		D3DXVECTOR4 color( 1.0f, 0.0f, 0.0f, 1.0f);
		//���[���h�s��̓]���B
		g_pEffect->SetMatrix("g_worldMatrix", &g_worldMatrix);
		//�r���[�s��̓]���B
		g_pEffect->SetMatrix("g_viewMatrix", &g_viewMatrix);				//@�J�����N���X����邽�߂̃q���g�B �J�����N���X�̃Q�b�^�[���g�p����悤�ɂ���B
		//�v���W�F�N�V�����s��̓]���B
		g_pEffect->SetMatrix("g_projectionMatrix", &g_projectionMatrix);	//@�J�����N���X����邽�߂̃q���g�B �J�����N���X�̃Q�b�^�[���g�p����悤�ɂ���B
		//��]�s���]���B
		g_pEffect->SetMatrix( "g_rotationMatrix", &g_rotationMatrix );
		//���C�g�̌�����]���B
		g_pEffect->SetVectorArray("g_diffuseLightDirection", g_diffuseLightDirection, LIGHT_NUM );
		//���C�g�̃J���[��]���B
		g_pEffect->SetVectorArray("g_diffuseLightColor", g_diffuseLightColor, LIGHT_NUM );
		//������ݒ�B
		g_pEffect->SetVector("g_ambientLight", &g_ambientLight);

		g_pEffect->CommitChanges();						//���̊֐����Ăяo�����ƂŁA�f�[�^�̓]�����m�肷��B�`����s���O�Ɉ�񂾂��Ăяo���B
		
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
 *@brief	X�t�@�C����ǂݍ���ŃW�I���g�����������B
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
	//�@�������݂��邩���ׂ�B
	if ((g_pMesh->GetFVF() & D3DFVF_NORMAL) == 0) {
		//�@�����Ȃ��̂ō쐬����B
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
