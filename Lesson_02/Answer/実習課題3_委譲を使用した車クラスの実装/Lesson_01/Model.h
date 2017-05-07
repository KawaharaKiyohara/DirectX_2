#pragma once
#pragma once

//���f���\���̋��ʏ����𔲂��o�������f���N���X�B
class Model
{
public:
	Model();
	virtual ~Model();
	void Init(const char* fineName);
	void UpdateWorldMatrix(D3DXVECTOR3 position);
	void Render();
	void Release();
private:
	void LoadEffectFile();
private:
	ID3DXEffect*			pEffect = NULL;
	D3DXMATRIX				worldMatrix;			//���[���h�s��B���f�����[�J����Ԃ���A���[���h��Ԃɕϊ�����s��B
	LPD3DXMESH				pMesh = NULL;
	LPDIRECT3DTEXTURE9*	 	pMeshTextures = NULL; 	//�e�N�X�`���B
	DWORD              	 	dwNumMaterials = 0L;   	//�}�e���A���̐��B
};

