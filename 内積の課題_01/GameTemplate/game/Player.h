#pragma once


class Player
{
public:
	Player();
	~Player();
	void Start();
	void Update();
	void Render();
	void ExecuteTurn(D3DXVECTOR3 moveDir);
	const D3DXVECTOR3& GetPosition() const
	{
		return position;
	}
private:
	SkinModel model;
	SkinModelData modelData;
	Animation animation;
	D3DXVECTOR3 position;			//���W
	D3DXQUATERNION rotation;		//��]	
	D3DXVECTOR3 currentDirection;	//���݂̕����B
	D3DXQUATERNION baseRot;
	int currentCourceNodeNo = 0;
	Light light;	
};

