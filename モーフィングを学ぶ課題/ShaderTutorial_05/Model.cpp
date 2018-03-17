#include "stdafx.h"
#include "Model.h"

//コンストラクタ
Model::Model()
{
	mesh = NULL;
	textures = NULL;
	numMaterial = 0;
	effect = NULL;
}
//デストラクタ
Model::~Model()
{
	Release();
}
//初期化。
void Model::Init(LPDIRECT3DDEVICE9 pd3dDevice, const char* fileName)
{
	Release();
	//Xファイルをロード。
	LPD3DXBUFFER pD3DXMtrlBuffer;
	//Xファイルのロード。
	D3DXLoadMeshFromX(fileName, D3DXMESH_SYSTEMMEM,
		pd3dDevice, NULL,
		&pD3DXMtrlBuffer, NULL, &numMaterial,
		&mesh);
	//法線が存在するか調べる。
	if ((mesh->GetFVF() & D3DFVF_NORMAL) == 0) {
		//法線がないので作成する。
		ID3DXMesh* pTempMesh = NULL;

		mesh->CloneMeshFVF(mesh->GetOptions(),
			mesh->GetFVF() | D3DFVF_NORMAL, g_pd3dDevice, &pTempMesh);

		D3DXComputeNormals(pTempMesh, NULL);
		mesh->Release();
		mesh = pTempMesh;

	}

	// マテリアルバッファを取得。
	D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();

	//テクスチャをロード。
	textures = new LPDIRECT3DTEXTURE9[numMaterial];
	for (DWORD i = 0; i < numMaterial; i++)
	{
		textures[i] = NULL;
		//テクスチャを作成する。
		D3DXCreateTextureFromFileA(pd3dDevice,
			d3dxMaterials[i].pTextureFilename,
			&textures[i]);
	}
	// マテリアルバッファを解放。
	pD3DXMtrlBuffer->Release();

	//シェーダーをコンパイル。
	LPD3DXBUFFER  compileErrorBuffer = NULL;
	//シェーダーをコンパイル。
	HRESULT hr = D3DXCreateEffectFromFile(
		pd3dDevice,
		"basic.fx",
		NULL,
		NULL,
		D3DXSHADER_SKIPVALIDATION,
		NULL,
		&effect,
		&compileErrorBuffer
		);
	if (hr != S_OK) {
		MessageBox(NULL, (char*)(compileErrorBuffer->GetBufferPointer()), "error", MB_OK);
		std::abort();
	}
}

//描画。
void Model::Render(
	LPDIRECT3DDEVICE9 pd3dDevice,
	D3DXMATRIX worldMatrix,
	D3DXMATRIX rotationMatrix,
	D3DXMATRIX viewMatrix,
	D3DXMATRIX projMatrix,
	D3DXVECTOR4* diffuseLightDirection,
	D3DXVECTOR4* diffuseLightColor,
	D3DXVECTOR4	 ambientLight,
	int numDiffuseLight
	)
{
	effect->SetTechnique("SkinModel");
	effect->Begin(NULL, D3DXFX_DONOTSAVESHADERSTATE);
	effect->BeginPass(0);

	//定数レジスタに設定するカラー。
	D3DXVECTOR4 color(1.0f, 0.0f, 0.0f, 1.0f);
	//ワールド行列の転送。
	effect->SetMatrix("g_worldMatrix", &worldMatrix);
	//ビュー行列の転送。
	effect->SetMatrix("g_viewMatrix", &viewMatrix);			//ビュー行列を転送。

	effect->SetMatrix("g_projectionMatrix", &projMatrix);	//プロジェクション行列の転送。

	effect->SetMatrix("g_rotationMatrix", &rotationMatrix);		//回転行列を転送。
															//ライトの向きを転送。
	effect->SetVectorArray("g_diffuseLightDirection", diffuseLightDirection, numDiffuseLight);
	//ライトのカラーを転送。
	effect->SetVectorArray("g_diffuseLightColor", diffuseLightColor, numDiffuseLight);
	//環境光を設定。
	effect->SetVector("g_ambientLight", &ambientLight);

	effect->CommitChanges();						//この関数を呼び出すことで、データの転送が確定する。描画を行う前に一回だけ呼び出す。

	for (DWORD i = 0; i < numMaterial; i++)
	{
		effect->SetTexture("g_diffuseTexture", textures[i]);
		// Draw the mesh subset
		mesh->DrawSubset(i);
	}

	effect->EndPass();
	effect->End();
}
//開放。
void Model::Release()
{
	//メッシュを開放。
	if (mesh != NULL) {
		mesh->Release();
		mesh = NULL;
	}
	//テクスチャを開放。
	if (textures != NULL) {
		for (unsigned int i = 0; i < numMaterial; i++) {
			if (textures[i] != NULL) {
				textures[i]->Release();
				textures[i] = NULL;
			}
		}
		delete[] textures;
		textures = NULL;
	}
	//エフェクトを開放。
	if (effect != NULL) {
		effect->Release();
		effect = NULL;
	}
}
//メッシュを取得。
LPD3DXMESH Model::GetMesh()
{
	return mesh;
}
//モーフィング処理の実行。
//morphTarget	モーフターゲット
//rate モーフィングレート。
void Model::Morphing(Model* morphTargetA, Model* morphTargetB, float rate)
{
	//モーフターゲットAのメッシュを取得。
	LPD3DXMESH targetMesh_A= morphTargetA->GetMesh();
	//モーフターゲットAの頂点バッファを取得する。
	LPDIRECT3DVERTEXBUFFER9 targetVertexBuffer_A;
	targetMesh_A->GetVertexBuffer(&targetVertexBuffer_A);
	//頂点バッファの定義を取得する。
	D3DVERTEXBUFFER_DESC desc;
	targetVertexBuffer_A->GetDesc(&desc);
	//頂点ストライドを計算する。
	int stride = desc.Size / targetMesh_A->GetNumVertices();
	
	//モーフターゲットBのメッシュを取得する。
	LPD3DXMESH targetMesh_B = morphTargetB->GetMesh();
	//モーフターゲットBの頂点バッファを取得する。
	LPDIRECT3DVERTEXBUFFER9 targetVertexBuffer_B;
	targetMesh_B->GetVertexBuffer(&targetVertexBuffer_B);

	//自分の頂点バッファを取得する。
	LPDIRECT3DVERTEXBUFFER9 vertexBuffer;
	mesh->GetVertexBuffer(&vertexBuffer);
	D3DXVECTOR3* vertexPos;
	D3DXVECTOR3* targetVertexPos_A;
	D3DXVECTOR3* targetVertexPos_B;
	//頂点バッファをロック。
	vertexBuffer->Lock(0, desc.Size, (void**)&vertexPos, D3DLOCK_DISCARD);
	targetVertexBuffer_A->Lock(0, desc.Size, (void**)&targetVertexPos_A, D3DLOCK_DISCARD);
	targetVertexBuffer_B->Lock(0, desc.Size, (void**)&targetVertexPos_B, D3DLOCK_DISCARD);
	for (int vertNo = 0; vertNo < targetMesh_A->GetNumVertices(); vertNo++) {
		///////////////////////////////////////////////////////////////////
		//ここに頂点モーフの処理を記述する。
		///////////////////////////////////////////////////////////////////
		*vertexPos = (*targetVertexPos_A) * (1.0f - rate) 
			       + (*targetVertexPos_B) * rate;
	
		//次の頂点へ0。
		char* p = (char*)vertexPos;

		p += stride;
		vertexPos = (D3DXVECTOR3*)p;
		p = (char*)targetVertexPos_A;
		p += stride;
		targetVertexPos_A = (D3DXVECTOR3*)p;
		p = (char*)targetVertexPos_B;
		p += stride;
		targetVertexPos_B = (D3DXVECTOR3*)p;
	}
	//頂点バッファをアンロック。
	vertexBuffer->Unlock();
	targetVertexBuffer_A->Unlock();
	targetVertexBuffer_B->Unlock();
	vertexBuffer->Release();
	targetVertexBuffer_A->Release();
	targetVertexBuffer_B->Release();
}