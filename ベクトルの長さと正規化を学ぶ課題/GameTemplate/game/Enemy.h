#pragma once
class Enemy
{
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
	D3DXVECTOR3 position;
	int timer = 0;
};

