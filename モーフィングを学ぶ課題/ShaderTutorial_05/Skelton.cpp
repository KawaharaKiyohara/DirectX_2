#include "stdafx.h"
#include "Skelton.h"

Skelton::Skelton()
{

}
Skelton::~Skelton()
{

}
void Skelton::CalcGlobalPose(Joint* joint, Joint* parentJoint)
{
	D3DXVECTOR4 pos;
	if (parentJoint != NULL) {
		D3DXVec3Transform(&pos, &joint->localPosition, &parentJoint->mWorld);
		joint->position.x = pos.x;
		joint->position.y = pos.y;
		joint->position.z = pos.z;
		D3DXQUATERNION qParentRotInv;
		D3DXQuaternionInverse(&qParentRotInv, &parentJoint->rotation);
		joint->rotation = joint->localRotation * parentJoint->rotation;
		D3DXMATRIX mRot, mTrans;
		D3DXMatrixRotationQuaternion(&mRot, &joint->rotation);
		D3DXMatrixTranslation(&mTrans, joint->position.x, joint->position.y, joint->position.z);
		joint->mWorld = mRot * mTrans;
	}
	else {
		D3DXMATRIX mRot, mTrans;
		D3DXMatrixRotationQuaternion(&mRot, &joint->localRotation);
		D3DXMatrixTranslation(&mTrans, joint->localPosition.x, joint->localPosition.y, joint->localPosition.z);
		joint->position = joint->localPosition;
		joint->rotation = joint->localRotation;
		joint->mWorld = mRot * mTrans;
	}
	for (auto child : joint->children) {
		CalcGlobalPose(&jointArray[child], joint);
	}
}
//グローバルポーズの計算。
void Skelton::CalcGlobalPose()
{
	if (jointArray.empty()) {
		return;
	}
	Joint* root = &jointArray.front();
	D3DXMATRIX rootMatrix;
	D3DXMatrixIdentity(&rootMatrix);
	//ルートから潜っていく
	CalcGlobalPose(root, NULL);
}
//関節を追加。
void Skelton::AddJoint(int parentNo, const Joint* joint)
{
	//関節追加時にはグローバルポーズを計算する。
	CalcGlobalPose();
	if (parentNo != -1) {
		//関節を追加。
		jointArray.push_back(*joint);
		Joint* newJoint = &jointArray.back();
		Joint* parentJoint = &jointArray[parentNo];
		int childNo = (int)jointArray.size() - 1;
		parentJoint->children.push_back((int)jointArray.size()-1);
		//ローカルポーズを計算する。(親の座標系でのポーズを計算する。)
		D3DXMATRIX mParentWorldInv;
		D3DXMatrixInverse(&mParentWorldInv, NULL, &parentJoint->mWorld);
		//まずは座標。
		D3DXVECTOR4 pos;
		D3DXVec3Transform(&pos, &joint->position, &mParentWorldInv);
		newJoint->localPosition.x = pos.x;
		newJoint->localPosition.y = pos.y;
		newJoint->localPosition.z = pos.z;
		
		//続いて回転。
		D3DXQUATERNION qParentRotInv;
		D3DXQuaternionInverse(&qParentRotInv, &parentJoint->rotation);
		newJoint->localRotation = newJoint->rotation * qParentRotInv;
	}
	else {
		//ルート
		jointArray.clear();
		jointArray.push_back(*joint);
		jointArray[0].localPosition = jointArray[0].position;
		jointArray[0].localRotation = jointArray[0].rotation;
	}
}
//ワールド行列を取得。
const D3DXMATRIX* Skelton::GetWorldMatrix(int jointNo)
{
	return &jointArray[jointNo].mWorld;
}
//関節を取得。
Joint* Skelton::GetJoint(int jointNo)
{
	return &jointArray[jointNo];
}