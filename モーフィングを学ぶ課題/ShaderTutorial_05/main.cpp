/*!
 *@brief	�V�F�[�_�[�̃`���[�g���A��00
 */
#include "stdafx.h"
#include "Camera.h"
#include "Model.h"

//-----------------------------------------------------------------------------
// �O���[�o���ϐ��B
//-----------------------------------------------------------------------------

const int				LIGHT_NUM = 4;
D3DXVECTOR4 			g_diffuseLightDirection[LIGHT_NUM];	//���C�g�̕����B
D3DXVECTOR4				g_diffuseLightColor[LIGHT_NUM];		//���C�g�̐F�B
D3DXVECTOR4				g_ambientLight;						//����

Camera camera;				//�J�����B

Model originalModel;		//�I���W�i�����f���B
Model morphTargetA;			//���[�t�^�[�Q�b�g�`�B
Model morphTargetB;			//���[�t�^�[�Q�b�g�a�B
float morphRate = 0.0f;		//���[�t���[�g�B
/*!-----------------------------------------------------------------------------
 *@brief	���C�g���X�V�B
 -----------------------------------------------------------------------------*/
void UpdateLight()
{
	//�����B
	g_ambientLight = D3DXVECTOR4(0.8f, 0.8f, 0.8f, 1.0f);
}
//-----------------------------------------------------------------------------
// Name: �Q�[�����������B
//-----------------------------------------------------------------------------
void Init()
{
	//���C�g���������B
	ZeroMemory(g_diffuseLightDirection, sizeof(g_diffuseLightDirection));
	ZeroMemory(g_diffuseLightColor, sizeof(g_diffuseLightColor));

	//�J�����̏������B
	camera.Init();

	originalModel.Init(g_pd3dDevice, "org.X");			//�I���W�i�����f�������[�h�B
	morphTargetA.Init(g_pd3dDevice, "morphTargetA.X");	//���[�t�^�[�Q�b�gA�����[�h�B
	morphTargetB.Init(g_pd3dDevice, "morphTargetB.X");	//���[�t�^�[�Q�b�gB�����[�h�B
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
	D3DXMATRIX mWorld;
	D3DXMatrixIdentity(&mWorld);
	//���j�e�B������`��B
	originalModel.Render(
		g_pd3dDevice,
		mWorld,
		mWorld,
		camera.GetViewMatrix(),
		camera.GetProjectionMatrix(),
		g_diffuseLightDirection,
		g_diffuseLightColor,
		g_ambientLight,
		LIGHT_NUM
	);
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
	if (GetAsyncKeyState(VK_LEFT)) {
		morphRate = max(0.0f, morphRate - 0.03f);
	}
	if (GetAsyncKeyState(VK_RIGHT)) {
		morphRate = min(1.0f, morphRate + 0.03f);
	}
	originalModel.Morphing(&morphTargetA, &morphTargetB, morphRate);
	//���C�g�̍X�V�B
	UpdateLight();
	//�J�����̍X�V
	camera.Update();
}
//-----------------------------------------------------------------------------
//�Q�[�����I������Ƃ��ɌĂ΂�鏈���B
//-----------------------------------------------------------------------------
void Terminate()
{
}
