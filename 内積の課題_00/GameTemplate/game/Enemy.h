#pragma once
class Enemy
{
private:
	enum EState {
		eState_Search,	//索敵状態。
		eState_Find,	//発見状態。
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
	D3DXVECTOR3		position;	//座標。
	D3DXVECTOR3		direction;	//進行方向。
	D3DXQUATERNION	rotation;	//回転。
	int moveFrameCount = 0;
	
	EState state;		//敵の状態。
};

