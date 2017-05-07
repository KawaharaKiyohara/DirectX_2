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
	//シェーダーをコンパイル。
	LoadEffectFile();

	position = { 0.0f, 0.0f, 0.0f };
	// ワールド行列を単位行列に初期化する。
	D3DXMatrixIdentity(&worldMatrix);

	LPD3DXBUFFER pD3DXMtrlBuffer;
	//Xファイルのロード。
	D3DXLoadMeshFromX("Car.x", D3DXMESH_SYSTEMMEM,
		g_pd3dDevice, NULL,
		&pD3DXMtrlBuffer, NULL, &dwNumMaterials,
		&pMesh);
	// マテリアルバッファを取得。
	D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();

	pMeshTextures = new LPDIRECT3DTEXTURE9[dwNumMaterials];

	for (DWORD i = 0; i < dwNumMaterials; i++)
	{
		pMeshTextures[i] = NULL;
		//テクスチャを作成する。
		D3DXCreateTextureFromFileA(g_pd3dDevice,
			d3dxMaterials[i].pTextureFilename,
			&pMeshTextures[i]);
	}
	// マテリアルバッファを解放。
	pD3DXMtrlBuffer->Release();
}

void Car::Update()
{

	//ワールド行列を更新。
	D3DXMatrixTranslation(&worldMatrix, position.x, position.y, position.z);
}
void Car::Render()
{
	//シェーダー適用開始。
	pEffect->SetTechnique("SkinModel");
	pEffect->Begin(NULL, D3DXFX_DONOTSAVESHADERSTATE);
	pEffect->BeginPass(0);

	//ワールド行列の転送。
	pEffect->SetMatrix("g_worldMatrix", &worldMatrix);
	//ビュー行列の転送。
	pEffect->SetMatrix("g_viewMatrix", &g_viewMatrix);
	//プロジェクション行列の転送。
	pEffect->SetMatrix("g_projectionMatrix", &g_projectionMatrix);
	//ライトの向きを転送。
	pEffect->SetVectorArray("g_diffuseLightDirection", g_diffuseLightDirection, LIGHT_NUM);
	//ライトのカラーを転送。
	pEffect->SetVectorArray("g_diffuseLightColor", g_diffuseLightColor, LIGHT_NUM);
	//環境光を設定。
	pEffect->SetVector("g_ambientLight", &g_ambientLight);

	for (DWORD i = 0; i < dwNumMaterials; i++)
	{
		pEffect->SetTexture("g_diffuseTexture", pMeshTextures[i]);
		pEffect->CommitChanges();						//この関数を呼び出すことで、データの転送が確定する。描画を行う前に一回だけ呼び出す。
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