#pragma once
#include "GameObject.h"
#include "ShaderTag.h"

// 前方宣言
class SkeletalMeshComponent;
class AttachMeshComponent;

class SwordObject : GameObject
{
public:
	SwordObject(const Vector3& offsetPos,
		        const Vector3& offsetRot,
		        const char* gpmeshFileName,
		        SkeletalMeshComponent* skelComp,
		        const char* boneName);

	~SwordObject();

	void UpdateGameObject(float deltaTime)override;

private:
	
	// アタッチ先のボーン名
	const char* mAttachBoneName;

	// SkeletalMeshComponentクラスのポインタ
	SkeletalMeshComponent* mSkelComp;

	// AttachMeshComponentクラスのポインタ
	AttachMeshComponent* mAttachComp;

	// 適用するシェーダーのタグ
	ShaderTag mShaderTag;
};