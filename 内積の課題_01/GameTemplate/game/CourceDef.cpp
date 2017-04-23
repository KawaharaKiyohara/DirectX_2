#include "stdafx.h"
#include "CourceDef.h"



//コース定義のデータ構造体。
struct CourceDefData{
	D3DXVECTOR3 startPos;		//始点。
	D3DXVECTOR3 endPos;			//終点。
};
//コース定義のデータテーブル。
static CourceDefData datas[] = {
	{
		D3DXVECTOR3(0.000000f, 0.000000f, 0.000000f),
		D3DXVECTOR3(0.000000f, 0.000000f, -36.480076f),
	},
	{
		D3DXVECTOR3(0.000000f, 0.000000f, -36.480076f),
		D3DXVECTOR3(44.080250f, 0.000000f, -104.641632f),
	},
	{
		D3DXVECTOR3(44.080250f, 0.000000f, -104.641632f),
		D3DXVECTOR3(128.082169f, 0.000000f, -129.202194f),
	},
	{
		D3DXVECTOR3(128.082169f, 0.000000f, -129.202194f),
		D3DXVECTOR3(170.723145f, 0.000000f, -196.723740f),
	},
};
CourceDef::CourceDef()
{
}


CourceDef::~CourceDef()
{
}
