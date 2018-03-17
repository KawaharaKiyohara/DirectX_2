/*!
 *@brief	�V�F�[�_�[�̃`���[�g���A��00
 */
#include "stdafx.h"
#include "Camera.h"
#include "Tiger.h"
#include "Light.h"
//-----------------------------------------------------------------------------
// ���C�g�֌W�̕ϐ��B
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// ����ȊO�̃O���[�o���ϐ��B
//-----------------------------------------------------------------------------
const int NUM_TIGER = 1;	//�g���̐��B
Camera camera;				//�J�����B
Tiger tiger[NUM_TIGER];		//�ՁB
Light light;				//���C�g�N���X�̃C���X�^���X�B



//-----------------------------------------------------------------------------
// Name: �Q�[�����������B
//-----------------------------------------------------------------------------
void Init()
{
	
	//�Ղ��������B
	for (int i = 0; i < NUM_TIGER; i++) {
		tiger[i].Init(g_pd3dDevice);
		int y = i / 10;
		int x = i % 10;
		tiger[i].SetPosition(D3DXVECTOR3(0.5f * x, 0.5f * y, 0.0f));
	}
	//�J�����̏������B
	camera.Init();

}
//-----------------------------------------------------------------------------
// Name: �`�揈���B
//-----------------------------------------------------------------------------
VOID Render()
{
	// ��ʂ��N���A�B
	g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 255), 1.0f, 0);
	//�V�[���̕`��J�n�B
	g_pd3dDevice->BeginScene();
	//�g����`��B
	for (int i = 0; i < NUM_TIGER; i++) {
		tiger[i].Render(
			g_pd3dDevice,
			camera.GetViewMatrix(),
			camera.GetProjectionMatrix(),
			light.GetDiffuseLightDirection(),
			light.g_diffuseLightColor,
			light.g_ambientLight,
			light.LIGHT_NUM
			);
	}
	
	// �V�[���̕`��I���B
	g_pd3dDevice->EndScene();
	// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�����ւ���B
	g_pd3dDevice->Present(NULL, NULL, NULL, NULL);
}
/*!-----------------------------------------------------------------------------
 *@brief	�X�V�����B
 -----------------------------------------------------------------------------*/
void Update()
{
	//���C�g�̍X�V�B
	light.UpdateLight();
	//�J�����̍X�V
	camera.Update();
	//�Ղ̍X�V�B
	for (int i = 0; i < NUM_TIGER; i++) {
		tiger[i].Update();
	}
}
//-----------------------------------------------------------------------------
//�Q�[�����I������Ƃ��ɌĂ΂�鏈���B
//-----------------------------------------------------------------------------
void Terminate()
{
	for (int i = 0; i < NUM_TIGER; i++) {
		tiger[i].Release();
	}
}
