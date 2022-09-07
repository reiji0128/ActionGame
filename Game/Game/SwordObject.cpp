#include "SwordObject.h"
#include "Game.h"
#include "Renderer.h"
#include "GraphicResourceManager.h"
#include "Mesh.h"
#include "SkeletalMeshComponent.h"
#include "AttachMeshComponent.h"

SwordObject::SwordObject(const Vector3& offsetPos,
	                     const Vector3& offsetRot,
	                     const char* gpmeshFileName,
	                     SkeletalMeshComponent* skelComp,
	                     const char* boneName)
	:GameObject(Tag::Weapon)
	, mAttachBoneName(nullptr)
	, mSkelComp(nullptr)
	, mAttachComp(nullptr)
	, mShaderTag(ShaderTag::SKINNED_SHADOW_MAP)
{
	mAttachBoneName = boneName;
	mSkelComp = skelComp;

	//メッシュのセット
	Mesh* mesh = GraphicResourceManager::GetMesh(gpmeshFileName);
	mAttachComp = new AttachMeshComponent(this, mSkelComp, mAttachBoneName, mShaderTag);
	mAttachComp->SetMesh(mesh);
	mAttachComp->SetOffsetPosition(offsetPos);
	mAttachComp->SetOffsetRotation(offsetRot);
}

SwordObject::~SwordObject()
{
}

void SwordObject::UpdateGameObject(float deltaTime)
{
}
