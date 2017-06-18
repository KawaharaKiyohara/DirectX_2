/*!
 * @brief	�Q�[��
 */
#include "stdafx.h"
#include "game.h"


/*!
 * @brief	�R���X�g���N�^�B
 */
Game::Game()
{
}
/*!
 * @brief	�f�X�g���N�^�B
 */
Game::~Game()
{
}
/*!
 * @brief	�Q�[�����N�����Ă����x�����Ă΂��֐��B
 */
void Game::Start()
{
	//�J�����������B
	camera.Init();
	camera.SetEyePt(D3DXVECTOR3(0.0f, 2.0f, 4.0f));
	camera.SetLookatPt(D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	toEyePos = camera.GetEyePt() - camera.GetLookatPt();
	//�����_���王�_�܂ł̃x�N�g�����v�Z�B
	camera.Update();

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

	//���f�������[�h�B
	modelData.LoadModelData("Assets/model/Unity.X", &animation);
	
	model.Init(&modelData);
	model.SetLight(&light);
	animation.PlayAnimation(0);

}
/*!
 * @brief	�X�V�B
 */
void Game::Update()
{
	//Y������̉�]�s����쐬�B
	D3DXMATRIX mRot;
	//�P�ʍs����쐬�B
	D3DXMatrixIdentity(&mRot);
	if (GetAsyncKeyState(VK_LEFT)) {
		D3DXMatrixRotationY(&mRot, 0.1f);
	}
	else if (GetAsyncKeyState(VK_RIGHT)) {
		D3DXMatrixRotationY(&mRot, -0.1f);
	}
	//toEyePos���񂷁B
	D3DXVec3TransformCoord(&toEyePos, &toEyePos, &mRot);

	//�����āA�㉺�ɉ񂷁B
	//�P�ʍs��ɂ��Ƃ��B
	D3DXMatrixIdentity(&mRot);
	//��]�����鎲�����߂�B
	D3DXVECTOR3 rotAxis;
	D3DXVECTOR3 up = { 0.0f, 1.0f, 0.0f };
	D3DXVec3Cross(&rotAxis, &up, &toEyePos);
	//��]���𐳋K������B
	D3DXVec3Normalize(&rotAxis, &rotAxis);
	if (GetAsyncKeyState(VK_UP)) {
		D3DXMatrixRotationAxis(&mRot, &rotAxis, 0.1f);
	}
	else if (GetAsyncKeyState(VK_DOWN)) {
		D3DXMatrixRotationAxis(&mRot, &rotAxis, -0.1f);
	}
	
	
	//toEyePos���񂷁B
	D3DXVECTOR3 toEyePosOld = toEyePos;
	D3DXVec3TransformCoord(&toEyePos, &toEyePos, &mRot);
	//�㉺�̉�]�͐�����݂���B
	D3DXVECTOR3 toEyePosNormalized;
	D3DXVec3Normalize(&toEyePosNormalized, &toEyePos);
	if (fabsf(toEyePosNormalized.x) < 0.5f && fabsf(toEyePosNormalized.z) < 0.5f) {
		//����𒴂��Ă���B
		toEyePos = toEyePosOld;
	}

	//�J�����̎��_�����肷��B
	D3DXVECTOR3 eyePos = camera.GetLookatPt();
	eyePos += toEyePos;
	camera.SetEyePt(eyePos);

	animation.Update(1.0f / 60.0f);
	camera.Update();
	model.UpdateWorldMatrix(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXQUATERNION(0.0f, 0.0f, 0.0f, 1.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f));
}
/*!
 * @brief	�`��B
 */
void Game::Render()
{
	model.Draw(&camera.GetViewMatrix(), &camera.GetProjectionMatrix());
}
