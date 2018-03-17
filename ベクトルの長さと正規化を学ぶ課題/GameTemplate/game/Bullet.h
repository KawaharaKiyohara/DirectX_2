#pragma once
class Bullet
{
public:
	Bullet();
	~Bullet();
	void Start(const D3DXVECTOR3& pos, const D3DXVECTOR3& moveSpeed);
	virtual bool Update();
	void Render();
	D3DXVECTOR3 GetPosition()
	{
		return position;
	}
protected:
	SkinModel model;
	static SkinModelData* modelData;
	Animation animation;
	Light light;
	D3DXVECTOR3 position;
	D3DXVECTOR3 moveSpeed;
	int life;		//éıñΩÅB
};

