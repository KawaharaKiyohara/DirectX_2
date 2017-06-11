#pragma once

//コース定義クラス。
struct CourceDefData {
	D3DXVECTOR3 startPos;		//始点。
	D3DXVECTOR3 endPos;			//終点。
};
//コース定義のデータテーブル。
#define COURCE_NODE_NUM		4
extern CourceDefData g_courceDatas[COURCE_NODE_NUM];

