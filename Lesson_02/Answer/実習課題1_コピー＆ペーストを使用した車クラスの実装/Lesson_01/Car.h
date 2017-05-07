#pragma once
class Car
{
public:
	/*!
	* @brief	�R���X�g���N�^�B
	*/
	Car();
	/*!
	* @brief	�f�X�g���N�^�B
	*/
	~Car();
	/*!
	* @brief	�������B
	*/
	void Init();
	/*!
	* @brief	�X�V�����B
	*/
	void Update();
	/*!
	* @brief	�`�揈���B
	*/
	void Render();
	/*!
	* @brief	�J���B
	*/
	void Release();
	/*!
	* @brief	���W�̐ݒ�B
	*/
	void SetPosition(D3DXVECTOR3 pos);
private:
	/*!
	* @brief	�G�t�F�N�g�t�@�C���̃��[�h�B
	*/
	void LoadEffectFile();
private:
	ID3DXEffect*			pEffect = NULL;
	D3DXMATRIX				worldMatrix;			//���[���h�s��B���f�����[�J����Ԃ���A���[���h��Ԃɕϊ�����s��B
	LPD3DXMESH				pMesh = NULL;
	LPDIRECT3DTEXTURE9*	 	pMeshTextures = NULL; 	//�e�N�X�`���B
	DWORD              	 	dwNumMaterials = 0L;   	//�}�e���A���̐��B
	D3DXVECTOR3				position;				//���W�B
};

