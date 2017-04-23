/*!
 *@brief	DirectX�U Lesson_01
 */
#include "stdafx.h"


//-----------------------------------------------------------------------------
// �O���[�o���ϐ��B
//-----------------------------------------------------------------------------
LPDIRECT3D9             g_pD3D = NULL;		
LPDIRECT3DDEVICE9       g_pd3dDevice = NULL;
ID3DXEffect*			g_pEffect = NULL;	

D3DXMATRIX				g_worldMatrix;		//���[���h�s��B���f�����[�J����Ԃ���A���[���h��Ԃɕϊ�����s��B
D3DXMATRIX				g_rotationMatrix;	//��]�s��B�@�����񂷂��߂ɕK�v�Ȃ̂ŕʓr�p�ӁB]

LPD3DXMESH				g_pMesh = NULL;
LPDIRECT3DTEXTURE9*	 	g_pMeshTextures = NULL; 	// Textures for our mesh
DWORD              	 	g_dwNumMaterials = 0L;   	// Number of mesh materials


static const int		LIGHT_NUM = 4;
D3DXVECTOR4 			g_diffuseLightDirection[LIGHT_NUM];	//���C�g�̕����B
D3DXVECTOR4				g_diffuseLightColor[LIGHT_NUM];		//���C�g�̐F�B
D3DXVECTOR4				g_ambientLight;						//����

D3DXMATRIX				g_viewMatrix;						//�r���[�s��B
D3DXMATRIX				g_projectionMatrix;					//�v���W�F�N�V�����s��B


/*!-----------------------------------------------------------------------------
 *@brief	�V�F�[�_�[�G�t�F�N�g�t�@�C��(*.fx)�����[�h�B
 -----------------------------------------------------------------------------*/
void LoadEffectFile()
{
	LPD3DXBUFFER  compileErrorBuffer = NULL;
	//�V�F�[�_�[���R���p�C���B
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
	//D3D�I�u�W�F�N�g���쐬����B
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	
	// D3D�f�o�C�X���쐬���邽�߂̃p�����[�^��ݒ肷��B
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	//D3D�f�o�C�X���쐬����B
	g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp, &g_pd3dDevice);

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
/*!-----------------------------------------------------------------------------
 *@brief	���C�g���X�V�B
 -----------------------------------------------------------------------------*/
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
	g_ambientLight = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
}
//-----------------------------------------------------------------------------
// Name: �`�揈���B
//-----------------------------------------------------------------------------
VOID Render()
{
	// ��ʂ��N���A�B
	g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 255), 1.0f, 0);
	
	if (SUCCEEDED(g_pd3dDevice->BeginScene()))
	{
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
		g_pEffect->SetMatrix("g_viewMatrix", &g_viewMatrix);
		//�v���W�F�N�V�����s��̓]���B
		g_pEffect->SetMatrix("g_projectionMatrix", &g_projectionMatrix);
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
/*!-----------------------------------------------------------------------------
*@brief	�J�����̏������B
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
 *@brief	X�t�@�C����ǂݍ���ŃW�I���g�����������B
 -----------------------------------------------------------------------------*/
void InitGeometry()
{
    LPD3DXBUFFER pD3DXMtrlBuffer;
    //X�t�@�C���̃��[�h�B
	D3DXLoadMeshFromX( "Tiger.x", D3DXMESH_SYSTEMMEM,
                                   g_pd3dDevice, NULL,
                                   &pD3DXMtrlBuffer, NULL, &g_dwNumMaterials,
                                   &g_pMesh );
    // �}�e���A���o�b�t�@���擾�B
    D3DXMATERIAL* d3dxMaterials = ( D3DXMATERIAL* )pD3DXMtrlBuffer->GetBufferPointer();
   
    g_pMeshTextures = new LPDIRECT3DTEXTURE9[g_dwNumMaterials];
   
    for( DWORD i = 0; i < g_dwNumMaterials; i++ )
    {
        g_pMeshTextures[i] = NULL;
        //�e�N�X�`�����쐬����B
		D3DXCreateTextureFromFileA( g_pd3dDevice,
                                    d3dxMaterials[i].pTextureFilename,
                                    &g_pMeshTextures[i] );
    }
    // �}�e���A���o�b�t�@������B
    pD3DXMtrlBuffer->Release();
}
/*!-----------------------------------------------------------------------------
 *@brief	�X�V�����B
 -----------------------------------------------------------------------------*/
void Update()
{
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
		"Lesson 01", NULL
	};
	RegisterClassEx(&wc);

	// Create the application's window
	HWND hWnd = CreateWindow("Lesson 01", "Lesson 01",
		WS_OVERLAPPEDWINDOW, 100, 100, 500, 500,
		NULL, NULL, wc.hInstance, NULL);

	// Direct3D���������B
	InitD3D(hWnd);
	//���f�����������B
	InitGeometry();
	//�G�t�F�N�g�t�@�C���̃��[�h�B
	LoadEffectFile();
	
	ZeroMemory( g_diffuseLightDirection, sizeof(g_diffuseLightDirection) );
	ZeroMemory( g_diffuseLightColor, sizeof(g_diffuseLightColor) );
			
	D3DXMatrixIdentity(&g_worldMatrix);
	D3DXMatrixIdentity(&g_rotationMatrix);
	//�J�����̏������B
	InitCamera();
	
	// Show the window
	ShowWindow(hWnd, SW_SHOWDEFAULT);
	UpdateWindow(hWnd);

	// �Q�[�����[�v
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
