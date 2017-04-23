/*!
 *@brief	�V�F�[�_�[�̃`���[�g���A��00
 */
#include "stdafx.h"
#include "Camera.h"
#include "Ball.h"
#include "Skelton.h"

//-----------------------------------------------------------------------------
// �O���[�o���ϐ��B
//-----------------------------------------------------------------------------

const int				LIGHT_NUM = 4;
D3DXVECTOR4 			g_diffuseLightDirection[LIGHT_NUM];	//���C�g�̕����B
D3DXVECTOR4				g_diffuseLightColor[LIGHT_NUM];		//���C�g�̐F�B
D3DXVECTOR4				g_ambientLight;						//����

Camera camera;				//�J�����B

const int NUM_BALL = 7;
Ball ball[NUM_BALL];	//�{�[���B
Skelton ballSkelton;	//�{�[���̃X�P���g���B

enum EJoint {
	eJointPelvis,
	eJointLeftFoot ,
	eJointRightFoot ,
	eJointBody,
	eJointHead,
	eJointLeftHand,
	eJointRightHand,
};
/*!-----------------------------------------------------------------------------
 *@brief	���C�g���X�V�B
 -----------------------------------------------------------------------------*/
void UpdateLight()
{
	g_diffuseLightDirection[0] = D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f);

	//�f�B�t���[�Y���C�g�B
	g_diffuseLightColor[0] = D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f);

//�����B
g_ambientLight = D3DXVECTOR4(0.2f, 0.2f, 0.2f, 1.0f);
}
//-----------------------------------------------------------------------------
// Name: �Q�[�����������B
//-----------------------------------------------------------------------------
void Init()
{
	//���C�g���������B
	ZeroMemory(g_diffuseLightDirection, sizeof(g_diffuseLightDirection));
	ZeroMemory(g_diffuseLightColor, sizeof(g_diffuseLightColor));


	//�X�P���g�����\�z�B
	//���Ձ@���ꂪ���[�g
	Joint joint;
	D3DXQuaternionIdentity(&joint.rotation);
	joint.position.x = 0.0f;
	joint.position.y = 0.0f;
	joint.position.z = 0.0f;
	ballSkelton.AddJoint(-1, &joint);
	//����
	joint.position.x = -0.5f;
	joint.position.y = -0.5f;
	joint.position.z = 0.0f;
	ballSkelton.AddJoint(0, &joint);
	//�E���B
	joint.position.x = 0.5f;
	joint.position.y = -0.5f;
	joint.position.z = 0.0f;
	ballSkelton.AddJoint(0, &joint);
	//����
	joint.position.x = 0.0f;
	joint.position.y = 0.5f;
	joint.position.z = 0.0f;
	ballSkelton.AddJoint(0, &joint);
	//��
	joint.position.x = 0.0f;
	joint.position.y = 1.0f;
	joint.position.z = 0.0f;
	ballSkelton.AddJoint(3, &joint);
	//����
	joint.position.x = -0.5f;
	joint.position.y = 0.5f;
	joint.position.z = 0.0f;
	ballSkelton.AddJoint(3, &joint);
	//�E��
	joint.position.x = 0.5f;
	joint.position.y = 0.5f;
	joint.position.z = 0.0f;
	ballSkelton.AddJoint(3, &joint);


	//�{�[�����������B
	for (int i = 0; i < NUM_BALL; i++) {
		ball[i].Init(&ballSkelton, i);
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
	for (int i = 0; i < NUM_BALL; i++) {
		ball[i].Render(
			g_pd3dDevice,
			camera.GetViewMatrix(),
			camera.GetProjectionMatrix(),
			g_diffuseLightDirection,
			g_diffuseLightColor,
			g_ambientLight,
			LIGHT_NUM
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
	UpdateLight();
	//�J�����̍X�V
	camera.Update();
	//�O���[�o���|�[�Y�̍X�V
	ballSkelton.CalcGlobalPose();
	//���Ղ𓮂���
	if (GetAsyncKeyState(VK_UP)) {
		Joint* joint = ballSkelton.GetJoint(eJointPelvis);
		joint->localPosition.y += 0.01f;
	}
	//���̂𓮂���
	if (GetAsyncKeyState('W')){
		Joint* joint = ballSkelton.GetJoint(eJointBody);
		joint->localPosition.y += 0.01f;
	}
	if (GetAsyncKeyState('A')) {
		Joint* joint = ballSkelton.GetJoint(eJointBody);
		static float angle = 0.0f;
		D3DXQuaternionRotationAxis(&joint->localRotation, &D3DXVECTOR3(0.0f, 1.0f, 0.0f), angle);
		angle+=0.01f;
	}

}
//-----------------------------------------------------------------------------
//�Q�[�����I������Ƃ��ɌĂ΂�鏈���B
//-----------------------------------------------------------------------------
void Terminate()
{
}
