#pragma once
class Enemy
{
private:
	enum EState {
		eState_Search,	//���G��ԁB
		eState_Find,	//������ԁB
	};
public:
	Enemy();
	~Enemy();
	void Start(const D3DXVECTOR3& pos);
	bool Update();
	void Render();
private:
	SkinModel model;
	static SkinModelData* modelData;
	Animation animation;
	Light light;
	D3DXVECTOR3		position;	//���W�B
	D3DXVECTOR3		direction;	//�i�s�����B
	D3DXQUATERNION	rotation;	//��]�B
	int moveFrameCount = 0;
	
	EState state;		//�G�̏�ԁB
};

