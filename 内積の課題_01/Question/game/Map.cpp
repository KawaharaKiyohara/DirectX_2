#include "stdafx.h"
#include "Map.h"
#include "Player.h"
#include "game.h"

namespace {
	
}
Map::Map()
{
}

Map::~Map()
{
	for (auto model : models) {
		delete model;
	}
	for (auto& it : modelMap) {
		delete it.second;
	}
}

//Start�֐��B
void Map::Start()
{
	//�n�ʂ����[�h�B
	SkinModelData* modelData = new SkinModelData;
	const char* groundModelDataPath = "Assets/model/ground.x";
	int hash = MakeHash(groundModelDataPath);	//���f���p�X����n�b�V���𐶐�����B
	modelMap.insert(std::pair<int, SkinModelData*>(hash, modelData));
	modelData->LoadModelData(groundModelDataPath, NULL);
	light.SetAmbientLight(D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f));
	SkinModel* model = new SkinModel;
	model->Init(modelData);
	model->SetLight(&light);
	models.push_back(model);
}
//�X�V�B
void Map::Update()
{
	for (auto& model : models) {
		model->UpdateWorldMatrix(
			D3DXVECTOR3(0.0f, 0.0f, 0.0f), 
			D3DXQUATERNION(0.0f, 0.0f, 0.0f, 1.0f), 
			D3DXVECTOR3(10.0f, 1.0f, 10.0f)
		);
	}
}
//�`��B
void Map::Render()
{
	const Camera& camera = game->GetCamera();
	for (auto& model : models) {
		model->Draw(&camera.GetViewMatrix(), &camera.GetProjectionMatrix());
	}
}