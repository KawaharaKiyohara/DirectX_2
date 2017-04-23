#pragma once

extern D3DXMATRIX			g_worldMatrix;
//-----------------------------------------------------------------------------
// ����̎��K�ł͋C�ɂ��Ȃ��Ă����O���[�o���ϐ��B
//-----------------------------------------------------------------------------
extern LPDIRECT3D9             g_pD3D;
extern LPDIRECT3DDEVICE9       g_pd3dDevice ;
extern ID3DXEffect*				g_pEffect;
extern D3DXMATRIX				g_viewMatrix;										//�r���[�s��B�J�����s��Ƃ������B
extern D3DXMATRIX				g_projectionMatrix;									//�v���W�F�N�V�����s��B�r���[��Ԃ���ˉe��Ԃɕϊ�����s��B


extern LPD3DXMESH				g_pMesh;
extern LPDIRECT3DTEXTURE9*	 	g_pMeshTextures; 	// Textures for our mesh
extern DWORD              	 	g_dwNumMaterials;   	// Number of mesh materials


const int		LIGHT_NUM = 4;
extern D3DXVECTOR4 			g_diffuseLightDirection[LIGHT_NUM];	//���C�g�̕����B
extern D3DXVECTOR4				g_diffuseLightColor[LIGHT_NUM];		//���C�g�̐F�B
extern D3DXVECTOR4				g_ambientLight;						//����

//-----------------------------------------------------------------------------
// ����̎��K�ł͋C�ɂ��Ȃ��Ă����֐��B
//-----------------------------------------------------------------------------
void LoadEffectFile();
HRESULT InitD3D(HWND hWnd);
void InitProjectionMatrix();
VOID Cleanup();
void UpdateLight();