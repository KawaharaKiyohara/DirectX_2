#pragma once

//マップクラス。
class Map
{
public:
	//コンストラクタ
	Map();
	//デストラクタ
	~Map();
	//Start関数。
	void Start();
	//描画。
	void Render();
	//更新。
	void Update();
private:
	std::vector<SkinModel*>			models;		//マップを構成するモデルのリスト
	std::map<int, SkinModelData*>	modelMap;	//モデルマップ。
	Light							light;		//ライト。
};

