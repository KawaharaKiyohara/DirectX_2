#include "stdafx.h"
#include "Tiger.h"


Tiger::Tiger()
{
}


Tiger::~Tiger()
{
	Release();
}

void Tiger::SetPosition(D3DXVECTOR3 pos)
{
	position = pos;
}

void Tiger::Move()
{
	if (GetAsyncKeyState(VK_UP)) {
		position.z += 0.05f;
	}
	if (GetAsyncKeyState(VK_DOWN)) {
		position.z -= 0.05f;
	}
	if (GetAsyncKeyState(VK_LEFT)) {
		position.x -= 0.05f;
	}
	if (GetAsyncKeyState(VK_RIGHT)) {
		position.x += 0.05f;
	}
}
void Tiger::Update()
{
	//�Ղ̈ړ������B
	Move();
	//Model�N���X��UpdateWorldMatrix���Ăяo���B
	UpdateWorldMatrix(position);
}