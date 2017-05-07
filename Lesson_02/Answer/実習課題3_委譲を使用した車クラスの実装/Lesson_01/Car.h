#pragma once
#include "Model.h"
class Car 
{
public:
	Car();
	~Car();
	/*!
	* @brief	�X�V�����B
	*/
	void Update();
	/*!
	* @brief	�����������B
	*/
	void Init();
	/*!
	* @brief	�`�揈���B
	*/
	void Render();
	/*!
	* @brief	�J�������B
	*/
	void Release();
	/*!
	* @brief	���W�̐ݒ�B
	*/
	void SetPosition(D3DXVECTOR3 pos);
private:
	D3DXVECTOR3 position;
	Model model;
};

