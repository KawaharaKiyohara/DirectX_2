#pragma once
#include <d3d9.h>
#include <d3dx9effect.h>
#pragma warning( disable : 4996 ) 
#include <strsafe.h>
#pragma warning( default : 4996 )
#include <cstdlib>

const int	LIGHT_NUM = 4;
extern D3DXVECTOR4 		g_diffuseLightDirection[LIGHT_NUM];	//���C�g�̕����B
extern D3DXVECTOR4		g_diffuseLightColor[LIGHT_NUM];		//���C�g�̐F�B
extern D3DXVECTOR4		g_ambientLight;						//����

extern D3DXMATRIX		g_viewMatrix;						//�r���[�s��B
extern  D3DXMATRIX		g_projectionMatrix;					//�v���W�F�N�V�����s��B