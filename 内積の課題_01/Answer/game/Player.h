#pragma once


class Player
{
public:
	Player();
	~Player();
	void Start();
	void Update();
	void Render();
	//void ExecuteTurn(D3DXVECTOR3 moveDir);
	const D3DXVECTOR3& GetPosition() const
	{
		return position;
	}

	const bool& GetPossible() const
	{
		return isPossibleMove;
	}
	const D3DXVECTOR3& Getposs()const
	{
		return anglepos;
	}
	const D3DXVECTOR3& Getmove()const
	{
		return move;
	}

private:
	SkinModel model;
	SkinModelData modelData;
	Animation animation;
	D3DXVECTOR3 position;			//座標
	D3DXQUATERNION rotation;		//回転	
	D3DXVECTOR3 currentDirection;	//現在の方向。
	D3DXQUATERNION baseRot;
	int currentCourceNodeNo = 0;
	bool isPossibleMove = true;
	D3DXVECTOR3 anglepos;
	D3DXVECTOR3 move;
	Light light;	
};

