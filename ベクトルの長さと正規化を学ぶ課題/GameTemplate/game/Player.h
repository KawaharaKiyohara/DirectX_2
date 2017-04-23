#pragma once
class Player
{
public:
	Player();
	~Player();
	void Start();
	void Update();
	void Render();
private:
	SkinModel model;
	SkinModelData modelData;
	Animation animation;
	D3DXVECTOR3 position;		//座標
	D3DXQUATERNION rotation;	//回転
	Light light;
	int bulletFireInterval;	//弾丸をもう一度打てるようになるまでのインターバル
};

