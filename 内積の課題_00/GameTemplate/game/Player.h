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
	D3DXVECTOR3 position;		//座標
	D3DXQUATERNION rotation;	//回転
	Light light;
	int bulletFireInterval;	//弾丸をもう一度打てるようになるまでのインターバル
	D3DXVECTOR3 direction_z;	//プレイヤーのZ方向。
	D3DXVECTOR3 direction_x;	//プレイヤーのX方向。
};

