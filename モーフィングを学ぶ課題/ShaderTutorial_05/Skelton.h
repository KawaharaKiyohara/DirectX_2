#pragma once

#include <vector>

//�֐߁B
struct Joint {
	D3DXVECTOR3			localPosition;	//�֐߂̃��[�J�����W�B
	D3DXQUATERNION		localRotation;	//�֐߂̃��[�J����]�B
	D3DXVECTOR3			position;		//�֐߂̃O���[�o�����W�B
	D3DXQUATERNION		rotation;		//�֐߂̃O���[�o����]�B
	std::vector<int>	children;		//�q���B
	D3DXMATRIX			mWorld;			//���[���h�s��A
};
class Skelton {
	std::vector<Joint>			jointArray;		//�Ԑڂ̔z��B
public:
	Skelton();
	~Skelton();
	//�O���[�o���|�[�Y�̌v�Z�B
	void CalcGlobalPose();
	//�֐߂�ǉ��B
	void AddJoint(int parentNo, const Joint* joint);
	//���[���h�s����擾�B
	const D3DXMATRIX* GetWorldMatrix(int jointNo);
	//�֐߂��擾�B
	Joint* GetJoint(int jointNo);
private:
	void CalcGlobalPose(Joint* joint, Joint* parentJoint);
};