#include "stdafx.h"
#include "GameCamera.h"
#include "Player.h"
#include "lib/Input.h"
#include "game.h"

GameCamera::GameCamera() :
	targetOffset(0.0f, 0.0f, 0.0f, 1.0f),
	cameraDir(0.0f, 0.0f, 0.0f)
{
}
GameCamera::~GameCamera()
{
}
void GameCamera::Start(Player* player)
{
	targetOffset.y = 1.0f;
	targetOffset.x = 0.0f;
	camera.SetAspect(1280.0f / 720.0f);
	camera.Init();
	this->player = player;
	UpdateCamera();

	target = player->GetPosition();
	eyePos = target;
	

}
//�J�������X�V�B

void GameCamera::UpdateCamera()
{
	//�����_�̓v���C���[�̍��W�B
	target = player->GetPosition();
	//���݂̃m�[�h�̌����𒲂ׂ�B
	D3DXVECTOR3 courceDir
		= g_courceDatas[currentCourceNodeNo].endPos
		- g_courceDatas[currentCourceNodeNo].startPos;
	//���݂̃m�[�h�̒��������߂�B
	float len = D3DXVec3Length(&courceDir);
	//�m�[�h�̌����𐳋K������B
	D3DXVec3Normalize(&courceDir, &courceDir);

	//�J�������R�[�X��̂ǂ̈ʒu�ɂ��邩�𒲂ׂ�B
	D3DXVECTOR3 cameraPosInCourceNode =
		eyePos
		- g_courceDatas[currentCourceNodeNo].startPos;
	float t = D3DXVec3Dot(&cameraPosInCourceNode, &courceDir);

	//�i�s�����̓R�[�X�̌����B
	isPossibleMove = true;
	if (t > len) {
		//���̃m�[�h�ցB
		currentCourceNodeNo++;
		isPossibleMove = false;
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
		D3DXVECTOR3 toEyePos = courceDir * -10.0f;
		toEyePos.y = 2.0f;
		eyePos = target + toEyePos;
	}

	camera.SetEyePt(eyePos);
	camera.SetLookatPt(target);
	camera.Update();
}
void GameCamera::PreUpdate()
{
	/*cameraDir = D3DXVECTOR3(toEyePos);
	cameraDir.y = 0.0f;
	D3DXVec3Normalize(&cameraDir, &cameraDir);*/
}
void GameCamera::Update()
{
	UpdateCamera();
}