#include "stdafx.h"
#include "Enemy.h"
#include "game.h"
#include "bullet.h"
#include "enemyBullet.h"

SkinModelData* Enemy::modelData = NULL;

Enemy::Enemy()
{
}


Enemy::~Enemy()
{
	delete modelData;
	modelData = NULL;
}
void Enemy::Start(const D3DXVECTOR3& pos)
{
	position = pos;
	//���C�g���������B
	light.SetDiffuseLightDirection(0, D3DXVECTOR4(0.707f, 0.0f, -0.707f, 1.0f));
	light.SetDiffuseLightDirection(1, D3DXVECTOR4(-0.707f, 0.0f, -0.707f, 1.0f));
	light.SetDiffuseLightDirection(2, D3DXVECTOR4(0.0f, 0.707f, -0.707f, 1.0f));
	light.SetDiffuseLightDirection(3, D3DXVECTOR4(0.0f, -0.707f, -0.707f, 1.0f));

	light.SetDiffuseLightColor(0, D3DXVECTOR4(0.2f, 0.2f, 0.2f, 1.0f));
	light.SetDiffuseLightColor(1, D3DXVECTOR4(0.2f, 0.2f, 0.2f, 1.0f));
	light.SetDiffuseLightColor(2, D3DXVECTOR4(0.3f, 0.3f, 0.3f, 1.0f));
	light.SetDiffuseLightColor(3, D3DXVECTOR4(0.2f, 0.2f, 0.2f, 1.0f));
	light.SetAmbientLight(D3DXVECTOR4(0.3f, 0.3f, 0.3f, 1.0f));

	if (modelData == NULL) {
		//���f�������[�h�B
		modelData = new SkinModelData;
		modelData->LoadModelData("Assets/model/DarkUnity.X", NULL);
	}

	model.Init(modelData);
	model.SetLight(&light);
}
bool Enemy::Update()
{
	timer++;
	if (timer > 20) {
		timer = 0;
		EnemyBullet* enemyBullet = new EnemyBullet();
		enemyBullet->Start(
			position, 
			D3DXVECTOR3(0.0f, -0.1f, 0.0f)
		);
		game->AddEnemyBullets(enemyBullet);
	}
	auto& bulletList = game->GetPlayerBullet();
	for (auto& bullet : bulletList) {
		D3DXVECTOR3 diff = 
			bullet->GetPosition() - position;
		float len = D3DXVec3Length(&diff);
		if (len < 0.5f) {
			//�e�ƏՓ˂��Ă���
			return false;
		}
	}
	//Question 1 ���S������s���B
	//�@�e�ۂ����X�g������Ă���K�v������B
	//  game.h��GetPlayerBullet�֐��ɃT���v���������B
	//�A�e�ۂƃG�l�~�[�̋����𒲂ׂ�K�v������܂��B
	//	bullet�N���X�ɂ�position�̃����o�����邯��
	//  private�ɂȂ��Ă���B
	//�B���S����Ƃ��ɖ߂�l��false�ɂ���B
	//Question 2 �G�l�~�[�����𔭎˂ł���悤�ɂ���B
	return true;
}
void Enemy::Render()
{
	model.UpdateWorldMatrix(position, D3DXQUATERNION(0.0f, 0.0f, 0.0f, 1.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	Camera* camera = game->GetCamera();
	model.Draw(&camera->GetViewMatrix(), &camera->GetProjectionMatrix());
}
