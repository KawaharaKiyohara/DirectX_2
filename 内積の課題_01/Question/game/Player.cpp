#include "stdafx.h"
#include "Player.h"
#include "game.h"
#include "lib/Input.h"

Player::Player()
{
	currentDirection = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
}


Player::~Player()
{
}

void Player::Start()
{
	//���C�g���������B
	light.SetDiffuseLightDirection(0, D3DXVECTOR4(0.707f, 0.0f, 0.707f, 1.0f));
	light.SetDiffuseLightDirection(1, D3DXVECTOR4(-0.707f, 0.0f, 0.707f, 1.0f));
	light.SetDiffuseLightDirection(2, D3DXVECTOR4(0.0f, 0.707f, 0.707f, 1.0f));
	light.SetDiffuseLightDirection(3, D3DXVECTOR4(0.0f, -0.707f, 0.707f, 1.0f));

	light.SetDiffuseLightColor(0, D3DXVECTOR4(0.2f, 0.2f, 0.2f, 1.0f));
	light.SetDiffuseLightColor(1, D3DXVECTOR4(0.2f, 0.2f, 0.2f, 1.0f));
	light.SetDiffuseLightColor(2, D3DXVECTOR4(0.3f, 0.3f, 0.3f, 1.0f));
	light.SetDiffuseLightColor(3, D3DXVECTOR4(0.2f, 0.2f, 0.2f, 1.0f));
	light.SetAmbientLight(D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f));

	//���f�������[�h�B
	modelData.LoadModelData("Assets/model/Unity.X", &animation);

	model.Init(&modelData);
	model.SetLight(&light);
	animation.PlayAnimation(1);
	position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	rotation = D3DXQUATERNION(0.0f, 0.0f, 0.0f, 1.0f);
}

void Player::Update()
{
	animation.Update(1.0f / 60.f);
	const float moveSpeed = 0.08f;
	D3DXVECTOR3 moveDir = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//�p�X�ړ����s���B
	//�v���C���[�̍��W���R�[�X��`�Ɏˉe���Ă����B
	D3DXVECTOR3 courceDir = 
		g_courceDatas[currentCourceNodeNo].endPos 
		- g_courceDatas[currentCourceNodeNo].startPos;
	//�R�[�X�̒��������߂Ă���A�R�[�X�̌��������߂�B
	float len = D3DXVec3Length(&courceDir);
	D3DXVec3Normalize(&courceDir, &courceDir);
	//�v���C���[���R�[�X��̂ǂ̈ʒu�ɂ��邩�𒲂ׂ�B
	D3DXVECTOR3 playerPosInCourceNode = 
		position 
		- g_courceDatas[currentCourceNodeNo].startPos;
	float t = D3DXVec3Dot(&playerPosInCourceNode, &courceDir);
	//�i�s�����̓R�[�X�̌����B
	moveDir = courceDir;
	bool isPossibleMove = true;
	if (t > len) {
		//���̃m�[�h�ցB
		currentCourceNodeNo++;
		if (currentCourceNodeNo >= COURCE_NODE_NUM) {
			//�I�[�܂ňړ������B
			//�ړ��͂����ł��Ȃ��B
			currentCourceNodeNo = COURCE_NODE_NUM;
			isPossibleMove = false;
		}
	}
	//�I���m�[�h�Ɍ������Ĉړ�����B
	if (isPossibleMove) {
		//�ړ��o������B
		position += moveDir * moveSpeed;
	}
	//���[���h�s����X�V�B
	model.UpdateWorldMatrix(position, rotation, D3DXVECTOR3(1.0f, 1.0f, 1.0f));
}
void Player::Render()
{
	const Camera& camera = game->GetCamera();
	model.Draw(&camera.GetViewMatrix(), &camera.GetProjectionMatrix());
}