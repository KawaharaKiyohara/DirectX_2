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
//���񏈗��B
void Player::ExecuteTurn(D3DXVECTOR3 moveDir)
{
	float length = D3DXVec3Length(&moveDir);
	if (length > 0.01f) {
		//�ړ����������͂���Ă���B
		//�i�s�����x�N�g���𐳋K������B
		D3DXVec3Normalize(&moveDir, &moveDir);
		turn.Update(currentDirection, moveDir);
		//��]�N�H�[�^�j�I�����X�V�B
		D3DXVECTOR3 baseDir(0.0f, 0.0f, 1.0f);
		float angle = D3DXVec3Dot(&currentDirection, &baseDir);
		//cos�̒l��-1.0�`1.0�܂łȂ̂�Clamp����B
		angle = min(angle, 1.0f);
		angle = max(angle, -1.0f);
		angle = acosf(angle);

		D3DXVECTOR3 rotAxis;
		D3DXVec3Cross(&rotAxis, &currentDirection, &baseDir);
		float length = D3DXVec3Length(&rotAxis);
		if (length < 0.01f) {
			rotAxis = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		}
		else {
			D3DXVec3Normalize(&rotAxis, &rotAxis);
		}
		D3DXVECTOR3 axisY(0.0f, 1.0f, 0.0f);
		D3DXQuaternionRotationAxis(&rotation, &rotAxis, angle);
	}
}
void Player::Update()
{
	animation.Update(1.0f / 60.f);
	const float moveSpeed = 0.08f;
	D3DXVECTOR3 moveDir = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	if (GetAsyncKeyState(VK_UP)) {
		moveDir.z -= 1.0f;
	}
	if (GetAsyncKeyState(VK_DOWN)) {
		moveDir.z += 1.0f;
	}
	if (GetAsyncKeyState(VK_LEFT)) {
		moveDir.x -= 1.0f;
	}
	if (GetAsyncKeyState(VK_RIGHT)) {
		moveDir.x += 1.0f;
	}
	//���񏈗��B
	ExecuteTurn(moveDir);

	moveDir.x *= -1.0f; //���j�e�B����񂪋t�������Ă���̂�
	position += moveDir * moveSpeed;
	//���[���h�s����X�V�B
	model.UpdateWorldMatrix(position, rotation, D3DXVECTOR3(1.0f, 1.0f, 1.0f));
}
void Player::Render()
{
	const Camera& camera = game->GetCamera();
	model.Draw(&camera.GetViewMatrix(), &camera.GetProjectionMatrix());
}