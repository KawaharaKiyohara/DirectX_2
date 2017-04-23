#pragma once
#include <d3d9.h>
#include <d3dx9effect.h>
#pragma warning( disable : 4996 ) 
#include <strsafe.h>
#pragma warning( default : 4996 )
#include <cstdlib>

const int	LIGHT_NUM = 4;
extern D3DXVECTOR4 		g_diffuseLightDirection[LIGHT_NUM];	//ライトの方向。
extern D3DXVECTOR4		g_diffuseLightColor[LIGHT_NUM];		//ライトの色。
extern D3DXVECTOR4		g_ambientLight;						//環境光

extern D3DXMATRIX		g_viewMatrix;						//ビュー行列。
extern  D3DXMATRIX		g_projectionMatrix;					//プロジェクション行列。