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
	D3DXMATRIX				worldMatrix;			//ワールド行列。モデルローカル空間から、ワールド空間に変換する行列。
	LPD3DXMESH				pMesh = NULL;
	LPDIRECT3DTEXTURE9*	 	pMeshTextures = NULL; 	//テクスチャ。
	DWORD              	 	dwNumMaterials = 0L;   	//マテリアルの数。
	D3DXVECTOR3				position;				//座標。
};

