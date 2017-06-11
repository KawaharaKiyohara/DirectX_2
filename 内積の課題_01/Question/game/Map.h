#pragma once

//�}�b�v�N���X�B
class Map
{
public:
	//�R���X�g���N�^
	Map();
	//�f�X�g���N�^
	~Map();
	//Start�֐��B
	void Start();
	//�`��B
	void Render();
	//�X�V�B
	void Update();
private:
	std::vector<SkinModel*>			models;		//�}�b�v���\�����郂�f���̃��X�g
	std::map<int, SkinModelData*>	modelMap;	//���f���}�b�v�B
	Light							light;		//���C�g�B
};

