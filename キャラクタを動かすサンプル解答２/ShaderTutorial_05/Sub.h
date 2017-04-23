#pragma once

extern D3DXMATRIX			g_worldMatrix;
//-----------------------------------------------------------------------------
// 今回の実習では気にしなくていいグローバル変数。
//-----------------------------------------------------------------------------
extern LPDIRECT3D9             g_pD3D;
extern LPDIRECT3DDEVICE9       g_pd3dDevice ;
extern ID3DXEffect*				g_pEffect;
extern D3DXMATRIX				g_viewMatrix;										//ビュー行列。カメラ行列とも言う。
extern D3DXMATRIX				g_projectionMatrix;									//プロジェクション行列。ビュー空間から射影空間に変換する行列。


extern LPD3DXMESH				g_pMesh;
extern LPDIRECT3DTEXTURE9*	 	g_pMeshTextures; 	// Textures for our mesh
extern DWORD              	 	g_dwNumMaterials;   	// Number of mesh materials


const int		LIGHT_NUM = 4;
extern D3DXVECTOR4 			g_diffuseLightDirection[LIGHT_NUM];	//ライトの方向。
extern D3DXVECTOR4				g_diffuseLightColor[LIGHT_NUM];		//ライトの色。
extern D3DXVECTOR4				g_ambientLight;						//環境光

//-----------------------------------------------------------------------------
// 今回の実習では気にしなくていい関数。
//-----------------------------------------------------------------------------
void LoadEffectFile();
HRESULT InitD3D(HWND hWnd);
void InitProjectionMatrix();
VOID Cleanup();
void UpdateLight();