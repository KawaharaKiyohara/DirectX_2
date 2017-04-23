#pragma once
class Player
{
public:
	Player();
	~Player();
	void Start();
	void Update();
	void Render();
	const D3DXVECTOR3& GetPosition() const
	{
		return position;
	}
private:
	SkinModel model;
	SkinModelData modelData;
	Animation animation;
	D3DXVECTOR3 position;		//���W
	D3DXQUATERNION rotation;	//��]
	Light light;
	int bulletFireInterval;	//�e�ۂ�������x�łĂ�悤�ɂȂ�܂ł̃C���^�[�o��
	D3DXVECTOR3 direction_z;	//�v���C���[��Z�����B
	D3DXVECTOR3 direction_x;	//�v���C���[��X�����B
};

