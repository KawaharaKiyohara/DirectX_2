#pragma once
#pragma once

//モデル表示の共通処理を抜き出したモデルクラス。
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
	D3DXMATRIX				worldMatrix;			//ワールド行列。モデルローカル空間から、ワールド空間に変換する行列。
	LPD3DXMESH				pMesh = NULL;
	LPDIRECT3DTEXTURE9*	 	pMeshTextures = NULL; 	//テクスチャ。
	DWORD              	 	dwNumMaterials = 0L;   	//マテリアルの数。
};

