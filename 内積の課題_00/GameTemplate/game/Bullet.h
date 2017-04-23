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
	D3DXVECTOR3		position;	//���W�B
	D3DXVECTOR3		moveSpeed;	//�ړ����x(�P�ʁF�b)
	Light			light;
	int				life;
};

