#include "stdafx.h"
#include "Model.h"


Model::Model()
{
}


Model::~Model()
{
}
void Model::LoadEffectFile()
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
void Model::Init(const char* fineName)
{
	//�V�F�[�_�[���R���p�C���B
	LoadEffectFile();

	// ���[���h�s���P�ʍs��ɏ���������B
	D3DXMatrixIdentity(&worldMatrix);

	LPD3DXBUFFER pD3DXMtrlBuffer;
	//X�t�@�C���̃��[�h�B
	D3DXLoadMeshFromX(fineName, D3DXMESH_SYSTEMMEM,
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
void Model::UpdateWorldMatrix(D3DXVECTOR3 position)
{
	//���[���h�s����X�V�B
	D3DXMatrixTranslation(&worldMatrix, position.x, position.y, position.z);
}
void Model::Render()
{
	//�V�F�[�_�[�K�p�J�n�B
	pEffect->SetTechnique("SkinModel");
	pEffect->Begin(NULL, D3DXFX_DONOTSAVESHADERSTATE);
	pEffect->BeginPass(0);

	//�萔���W�X�^�ɐݒ肷��J���[�B
	D3DXVECTOR4 color(1.0f, 0.0f, 0.0f, 1.0f);
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

	pEffect->CommitChanges();						//���̊֐����Ăяo�����ƂŁA�f�[�^�̓]�����m�肷��B�`����s���O�Ɉ�񂾂��Ăяo���B

													// Meshes are divided into subsets, one for each material. Render them in
													// a loop
	for (DWORD i = 0; i < dwNumMaterials; i++)
	{
		pEffect->SetTexture("g_diffuseTexture", pMeshTextures[i]);
		// Draw the mesh subset
		pMesh->DrawSubset(i);
	}

	pEffect->EndPass();
	pEffect->End();
}
void Model::Release()
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