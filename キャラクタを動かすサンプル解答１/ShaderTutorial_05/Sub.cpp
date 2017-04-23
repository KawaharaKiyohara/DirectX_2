#include <d3d9.h>
#include <d3dx9effect.h>
#pragma warning( disable : 4996 ) 
#include <strsafe.h>
#pragma warning( default : 4996 )
#include <cstdlib>
#include "sub.h"

LPDIRECT3D9             g_pD3D = NULL;
LPDIRECT3DDEVICE9       g_pd3dDevice = NULL;
ID3DXEffect*			g_pEffect = NULL;
D3DXMATRIX				g_viewMatrix;										//�r���[�s��B�J�����s��Ƃ������B
D3DXMATRIX				g_projectionMatrix;									//�v���W�F�N�V�����s��B�r���[��Ԃ���ˉe��Ԃɕϊ�����s��B


LPD3DXMESH				g_pMesh = NULL;
LPDIRECT3DTEXTURE9*	 	g_pMeshTextures = NULL; 	// Textures for our mesh
DWORD              	 	g_dwNumMaterials = 0L;   	// Number of mesh materials


D3DXVECTOR4 			g_diffuseLightDirection[LIGHT_NUM];	//���C�g�̕����B
D3DXVECTOR4				g_diffuseLightColor[LIGHT_NUM];		//���C�g�̐F�B
D3DXVECTOR4				g_ambientLight;						//����

/*!
*@brief	�V�F�[�_�[�G�t�F�N�g�t�@�C��(*.fx)�����[�h�B
* ����̎��K�ł͋C�ɂ��Ȃ��Ă����֐��B
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
//* ����̎��K�ł͋C�ɂ��Ȃ��Ă����֐��B
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
*@brief	�v���W�F�N�V�����s��̏������B
* ����̎��K�ł͋C�ɂ��Ȃ��Ă����֐��B
*/
void InitProjectionMatrix()
{
	D3DXMatrixIdentity(&g_worldMatrix);

	D3DXVECTOR3 vEyePt(0.0f, 10.0f, -15.0f);
	D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);
	D3DXMATRIXA16 matView;
	D3DXMatrixLookAtLH(&g_viewMatrix, &vEyePt, &vLookatPt, &vUpVec);

	D3DXMATRIXA16 matProj;
	D3DXMatrixPerspectiveFovLH(&g_projectionMatrix, D3DX_PI / 4, 1280.0f / 720.0f, 1.0f, 100.0f);
}


//-----------------------------------------------------------------------------
// Name: Cleanup()
// Desc: Releases all previously initialized objects
// ����̎��K�ł͋C�ɂ��Ȃ��Ă����֐��B
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
* ����̎��K�ł͋C�ɂ��Ȃ��Ă����֐��B
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