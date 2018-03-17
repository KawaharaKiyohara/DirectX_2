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
	animation.Update(1.0f / 60.0f);
	//���_���璍���_�����Z����toPos�����߂�B
	D3DXVECTOR3 toPos = camera.GetEyePt() - camera.GetLookatPt();
	//��]�s����쐬����B
	D3DXMATRIX mRot;
	D3DXMatrixIdentity(&mRot);
	if (GetAsyncKeyState(VK_LEFT)) {
		D3DXMatrixRotationY(&mRot,0.1f);
	}
	if (GetAsyncKeyState(VK_RIGHT)) {
		D3DXMatrixRotationY(&mRot, -0.1f);
	}
	D3DXVec3TransformCoord(&toPos, &toPos, &mRot);
	//�V�������_�����߂Đݒ肷��B
	D3DXVECTOR3 pos = camera.GetLookatPt() + toPos;
	camera.SetEyePt(pos);

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
