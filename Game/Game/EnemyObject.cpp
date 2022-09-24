#include "EnemyObject.h"
#include "Game.h"
#include "Renderer.h"
#include "GraphicResourceManager.h"
#include "Mesh.h"
#include "Skeleton.h"
#include "SkeletalMeshComponent.h"
#include "BoxCollider.h"
#include "LineCollider.h"
#include "Animation.h"
#include "ShaderTag.h"

#include "EnemyStaeBase.h"
#include "EnemyStateIdle.h"

EnemyObject::EnemyObject(const Vector3& pos,
	const float& scale,
	const char* gpmeshFileName,
	const char* gpskelFileName)
	:GameObject(Tag::Enemy)
	,mSkeltalMeshComp(nullptr)
	,mBoxCollider(nullptr)
	,mLineCollider(nullptr)
	,mNowState()
	,mNextState()
	,mUseShaderTag(ShaderTag::SKINNED_SHADOW_MAP)
{
	SetPosition(pos);

	SetScale(scale);

	// メッシュの読み込み・セット
	Mesh* mesh = GraphicResourceManager::LoadMesh(gpmeshFileName);
	mSkeltalMeshComp = new SkeletalMeshComponent(this, mUseShaderTag);
	mSkeltalMeshComp->SetMesh(mesh);

	// スケルトンの読み込み
	mSkeltalMeshComp->SetSkeleton(GraphicResourceManager::LoadSkeleton(gpskelFileName));

	// アニメーションの読み込み
	mAnimations.resize(static_cast<unsigned int>(EnemyState::STATE_NUM));
	mAnimations[static_cast<unsigned int>(EnemyState::STATE_IDLE)] = GraphicResourceManager::LoadAnimation("Assets/Dragon/DRAGON_idle.gpanim", true);


	// アイドル状態のアニメーションをセット
	mSkeltalMeshComp->PlayAnimation(mAnimations[static_cast<unsigned int>(EnemyState::STATE_IDLE)]);

	// アクターステートプールの初期化
	mStatePools.push_back(new EnemyStateIdle);

	// 当たり判定のセット
	AABB box = mesh->GetCollisionBox();
	box.Scaling(0.5f, 0.5f, 0.f);
	mBoxCollider = new BoxCollider(this);
	mBoxCollider->SetObjectBox(box);

	// ラインコライダーのセット
	mLineCollider = new LineCollider(this);
	Line line(Vector3(0, 0, 50.0f), Vector3(0, 0, -20.0f));
	mLineCollider->SetLine(line);
}

EnemyObject::~EnemyObject()
{
}

void EnemyObject::UpdateGameObject(float deltaTime)
{
}

void EnemyObject::OnCollisionEnter(ColliderComponent* ownCollider, ColliderComponent* otherCollider)
{
}
