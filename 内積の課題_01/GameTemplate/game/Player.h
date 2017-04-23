#pragma once

#include "Turn.h"

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
	D3DXVECTOR3 position;			//À•W
	D3DXQUATERNION rotation;		//‰ñ“]	
	D3DXVECTOR3 currentDirection;	//Œ»İ‚Ì•ûŒüB
	D3DXQUATERNION baseRot;
	Light light;
	Turn  turn;						//ù‰ñˆ—B
};

