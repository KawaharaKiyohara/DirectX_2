#pragma once
class Tiger
{
public:
	/*!
	 * @brief	コンストラクタ。
	 */
	Tiger();
	/*!
	 * @brief	デストラクタ。
	 */
	~Tiger();
	/*!
	 * @brief	初期化。
	 */
	void Init();
	/*!
	 * @brief	更新処理。
	 */
	void Update();
	/*!
	 * @brief	描画処理。
	 */
	void Render();
	/*!
	 * @brief	開放。
	 */
	void Release();
	/*!
	 * @brief	座標の設定。
	 */
	void SetPosition(D3DXVECTOR3 pos);
private:
	/*!
	 * @brief	エフェクトファイルのロード。
	 */
	void LoadEffectFile();
	/*!
	 * @brief	移動処理。
	 */
	void Move();
private:
	ID3DXEffect*			pEffect = NULL;
	D3DXMATRIX				worldMatrix;			//ワールド行列。モデルローカル空間から、ワールド空間に変換する行列。
	LPD3DXMESH				pMesh = NULL;
	LPDIRECT3DTEXTURE9*	 	pMeshTextures = NULL; 	//テクスチャ。
	DWORD              	 	dwNumMaterials = 0L;   	//マテリアルの数。
	D3DXVECTOR3				position;				//座標。
};

