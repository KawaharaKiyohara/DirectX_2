#pragma once

#include <vector>

//関節。
struct Joint {
	D3DXVECTOR3			localPosition;	//関節のローカル座標。
	D3DXQUATERNION		localRotation;	//関節のローカル回転。
	D3DXVECTOR3			position;		//関節のグローバル座標。
	D3DXQUATERNION		rotation;		//関節のグローバル回転。
	std::vector<int>	children;		//子供。
	D3DXMATRIX			mWorld;			//ワールド行列、
};
class Skelton {
	std::vector<Joint>			jointArray;		//間接の配列。
public:
	Skelton();
	~Skelton();
	//グローバルポーズの計算。
	void CalcGlobalPose();
	//関節を追加。
	void AddJoint(int parentNo, const Joint* joint);
	//ワールド行列を取得。
	const D3DXMATRIX* GetWorldMatrix(int jointNo);
	//関節を取得。
	Joint* GetJoint(int jointNo);
private:
	void CalcGlobalPose(Joint* joint, Joint* parentJoint);
};