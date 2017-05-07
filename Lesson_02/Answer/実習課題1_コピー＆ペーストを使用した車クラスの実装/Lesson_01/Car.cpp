#include "stdafx.h"
#include "Car.h"



Car::Car()
{
}


Car::~Car()
{
	Release();
}

void Car::SetPosition(D3DXVECTOR3 pos)
{
	position = pos;
}
void Car::LoadEffectFile()
{
	LPD3DXBUFFER  compileErrorBuffer = NULL;
	HRESULT hr = D3DXCreateEffectFromFile(
		g_pd3dDevice,
		"basic.fx",
		NULL,
		NULL,
		D3DXSHADER_SKIPVALIDATION,
		NULL,
		&pEffect,
		&compileErrorBuffer
	);
	if (FAILED(hr)) {
		MessageBox(NULL, (char*)(compileErrorBuffer->GetBufferPointer()), "error", MB_OK);
		std::abort();
	}
}

void Car::Init()
{
	//�V�F�[�_�[���R���p�C���B
	LoadEffectFile();

	position = { 0.0f, 0.0f, 0.0f };
	// ���[���h�s���P�ʍs��ɏ���������B
	D3DXMatrixIdentity(&worldMatrix);

	LPD3DXBUFFER pD3DXMtrlBuffer;
	//X�t�@�C���̃��[�h�B
	D3DXLoadMeshFromX("Car.x", D3DXMESH_SYSTEMMEM,
		g_pd3dDevice, NULL,
		&pD3DXMtrlBuffer, NULL, &dwNumMaterials,
		&pMesh);
	// �}�e���A���o�b�t�@���擾�B
	D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();

	pMeshTextures = new LPDIRECT3DTEXTURE9[dwNumMaterials];

	for (DWORD i = 0; i < dwNumMaterials; i++)
	{
		pMeshTextures[i] = NULL;
		//�e�N�X�`�����쐬����B
		D3DXCreateTextureFromFileA(g_pd3dDevice,
			d3dxMaterials[i].pTextureFilename,
			&pMeshTextures[i]);
	}
	// �}�e���A���o�b�t�@������B
	pD3DXMtrlBuffer->Release();
}

void Car::Update()
{

	//���[���h�s����X�V�B
	D3DXMatrixTranslation(&worldMatrix, position.x, position.y, position.z);
}
void Car::Render()
{
	//�V�F�[�_�[�K�p�J�n�B
	pEffect->SetTechnique("SkinModel");
	pEffect->Begin(NULL, D3DXFX_DONOTSAVESHADERSTATE);
	pEffect->BeginPass(0);

	//���[���h�s��̓]���B
	pEffect->SetMatrix("g_worldMatrix", &worldMatrix);
	//�r���[�s��̓]���B
	pEffect->SetMatrix("g_viewMatrix", &g_viewMatrix);
	//�v���W�F�N�V�����s��̓]���B
	pEffect->SetMatrix("g_projectionMatrix", &g_projectionMatrix);
	//���C�g�̌�����]���B
	pEffect->SetVectorArray("g_diffuseLightDirection", g_diffuseLightDirection, LIGHT_NUM);
	//���C�g�̃J���[��]���B
	pEffect->SetVectorArray("g_diffuseLightColor", g_diffuseLightColor, LIGHT_NUM);
	//������ݒ�B
	pEffect->SetVector("g_ambientLight", &g_ambientLight);

	for (DWORD i = 0; i < dwNumMaterials; i++)
	{
		pEffect->SetTexture("g_diffuseTexture", pMeshTextures[i]);
		pEffect->CommitChanges();						//���̊֐����Ăяo�����ƂŁA�f�[�^�̓]�����m�肷��B�`����s���O�Ɉ�񂾂��Ăяo���B
		// Draw the mesh subset
		pMesh->DrawSubset(i);
	}

	pEffect->EndPass();
	pEffect->End();
}
void Car::Release()
{
	if (pMeshTextures != NULL) {
		for (int i = 0; i < dwNumMaterials; i++) {
			pMeshTextures[i]->Release();
		}
		delete[] pMeshTextures;
		pMeshTextures = NULL;
	}
	if (pMesh != NULL) {
		pMesh->Release();
		pMesh = NULL;
	}
	if (pEffect != NULL) {
		pEffect->Release();
		pEffect = NULL;
	}
}