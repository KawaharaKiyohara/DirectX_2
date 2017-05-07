#pragma once
class Tiger
{
public:
	Tiger();
	~Tiger();
	void Init();
	void Update();
	void Render();
	void Release();
	void SetPosition(D3DXVECTOR3 pos);
private:
	ID3DXEffect*			pEffect = NULL;
	D3DXMATRIX				worldMatrix;			//���[���h�s��B���f�����[�J����Ԃ���A���[���h��Ԃɕϊ�����s��B
	LPD3DXMESH				pMesh = NULL;
	LPDIRECT3DTEXTURE9*	 	pMeshTextures = NULL; 	//�e�N�X�`���B
	DWORD              	 	dwNumMaterials = 0L;   	//�}�e���A���̐��B
	D3DXVECTOR3				position;				//���W�B
};

