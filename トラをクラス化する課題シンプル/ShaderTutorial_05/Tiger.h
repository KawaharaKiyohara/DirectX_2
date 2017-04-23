#pragma once

//�ՃN���X�B
class Tiger {
public:
	//�R���X�g���N�^
	Tiger();
	//�f�X�g���N�^
	~Tiger();
	//�������B
	void Init( LPDIRECT3DDEVICE9 pd3dDevice );
	//�X�V�B
	void Update();
	//�`��B
	void Tiger::Render(
		LPDIRECT3DDEVICE9 pd3dDevice,
		D3DXMATRIX viewMatrix,
		D3DXMATRIX projMatrix,
		D3DXVECTOR4* diffuseLightDirection,
		D3DXVECTOR4* diffuseLightColor,
		D3DXVECTOR4	 ambientLight,
		int numDiffuseLight
	);
	//�J���B
	void Release();
	//���W��ݒ�B
	void SetPosition(D3DXVECTOR3 pos);
private:
	D3DXVECTOR3				position;		//���W�B
	LPD3DXMESH				mesh;			//���b�V���B
	LPDIRECT3DTEXTURE9*		textures;		//�e�N�X�`���B
	DWORD					numMaterial;	//�}�e���A���̐��B
	ID3DXEffect*			effect;			//�G�t�F�N�g�B
	D3DXMATRIX				mWorld;			//���[���h�s��B
	D3DXMATRIX				mRotation;		//��]�s��B
};