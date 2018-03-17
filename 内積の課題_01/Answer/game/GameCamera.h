#pragma once

class Player;

//�Q�[���J����
class GameCamera
{
public:
	GameCamera();
	~GameCamera();
	void Start(Player* player);
	void PreUpdate();
	void Update();
	//�J�������擾�B
	const Camera& GetCamera() const
	{
		return camera;
	}
	//�J�����̕������擾�B
	D3DXVECTOR3 GetCameraDir() const
	{
		return cameraDir;
	}
private:
	//�J�������X�V�B
	void UpdateCamera();
private:
	Camera		camera;
	Player*		player;			//�Ǐ]����v���C���[�B
	D3DXVECTOR4 targetOffset;	//�����_�̃I�t�Z�b�g�B
//	D3DXVECTOR4 toEyePos;		//�����_���王�_�܂ł̃x�N�g���B	
	D3DXVECTOR3 cameraDir;		//�J�����̕����B
	int currentCourceNodeNo = 0;
	D3DXVECTOR3 target;
	D3DXVECTOR3 eyePos;
	bool isPossibleMove = true;
};

