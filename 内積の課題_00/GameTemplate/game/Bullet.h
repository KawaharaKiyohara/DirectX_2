#pragma once
class Bullet
{
public:
	Bullet();
	~Bullet();
	void Start(const D3DXVECTOR3& pos, const D3DXVECTOR3& moveSpeed);
	bool Update();
	void Render();
	const D3DXVECTOR3& GetPosition() const
	{
		return position;
	}
private:
	SkinModel model;
	static SkinModelData* modelData;
	D3DXVECTOR3		position;	//座標。
	D3DXVECTOR3		moveSpeed;	//移動速度(単位：秒)
	Light			light;
	int				life;
};

