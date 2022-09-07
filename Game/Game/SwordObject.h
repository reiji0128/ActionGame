#pragma once
#include "GameObject.h"
#include "ShaderTag.h"

// �O���錾
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
	
	// �A�^�b�`��̃{�[����
	const char* mAttachBoneName;

	// SkeletalMeshComponent�N���X�̃|�C���^
	SkeletalMeshComponent* mSkelComp;

	// AttachMeshComponent�N���X�̃|�C���^
	AttachMeshComponent* mAttachComp;

	// �K�p����V�F�[�_�[�̃^�O
	ShaderTag mShaderTag;
};