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
#include "HitPointGauge.h"

#include "EnemyStaeBase.h"
#include "EnemyStateIdle.h"
#include "EnemyStatePatrol.h"
#include "EnemyStateRun.h"
#include "EnemyStateAttackHand.h"
#include "EnemyStateAttackBite.h"
#include "EnemyStateDeath.h"

EnemyObject::EnemyObject(const Vector3& pos,const float& scale,const char* gpmeshFileName,const char* gpskelFileName)
	:GameObject(Tag::Enemy)
	,mHP(mMaxHP)
	,mSkeltalMeshComp(nullptr)
	,mBoxCollider(nullptr)
	,mLineCollider(nullptr)
	,mNowState()
	,mNextState()
	,mUseShaderTag(ShaderTag::SKINNED_SHADOW_MAP)
{
	SetPosition(pos);

	SetScale(scale);

	SetDirection(Vector3(-1, 0, 0));

	// メッシュの読み込み・セット
	Mesh* mesh = GraphicResourceManager::LoadMesh(gpmeshFileName);
	mSkeltalMeshComp = new SkeletalMeshComponent(this, mUseShaderTag);
	mSkeltalMeshComp->SetMesh(mesh);

	// スケルトンの読み込み
	mSkeltalMeshComp->SetSkeleton(GraphicResourceManager::LoadSkeleton(gpskelFileName));

	// アニメーションの読み込み
	mAnimations.resize(static_cast<unsigned int>(EnemyState::STATE_NUM));
	mAnimations[static_cast<unsigned int>(EnemyState::STATE_IDLE)]        = GraphicResourceManager::LoadAnimation("Assets/Dragon/DRAGON_idle.gpanim"                     , true);
	mAnimations[static_cast<unsigned int>(EnemyState::STATE_PATROL)]      = GraphicResourceManager::LoadAnimation("Assets/Dragon/ANIM_DRAGON_walk.gpanim"                , true);
	mAnimations[static_cast<unsigned int>(EnemyState::STATE_RUN)]         = GraphicResourceManager::LoadAnimation("Assets/Dragon/ANIM_DRAGON_walk.gpanim"                , true);
	mAnimations[static_cast<unsigned int>(EnemyState::STATE_ATTACK_HAND)] = GraphicResourceManager::LoadAnimation("Assets/Dragon/ANIM_MOUNTAIN_DRAGON_AttackHand.gpanim" , false);
	mAnimations[static_cast<unsigned int>(EnemyState::STATE_ATTACK_BITE)] = GraphicResourceManager::LoadAnimation("Assets/Dragon/ANIM_DRAGON_bite.gpanim"                , false);
	mAnimations[static_cast<unsigned int>(EnemyState::STATE_FIRE_BREATH)] = GraphicResourceManager::LoadAnimation("Assets/Dragon/ANIM_DRAGON_spreadFire.gpanim"          , false);
	mAnimations[static_cast<unsigned int>(EnemyState::STATE_FIRE_BALL)]   = GraphicResourceManager::LoadAnimation("Assets/Dragon/STATE_FIRE_BREATH.gpanim"               , false);
	mAnimations[static_cast<unsigned int>(EnemyState::STATE_DEATH)]       = GraphicResourceManager::LoadAnimation("Assets/Dragon/ANIM_DRAGON_death.gpanim"               , false);


	// アイドル状態のアニメーションをセット
	mSkeltalMeshComp->PlayAnimation(mAnimations[static_cast<unsigned int>(EnemyState::STATE_IDLE)]);

	// アクターステートプールの初期化
	mStatePools.push_back(new EnemyStateIdle);
	mStatePools.push_back(new EnemyStatePatrol);
	mStatePools.push_back(new EnemyStateRun);
	mStatePools.push_back(new EnemyStateAttackHand);
	mStatePools.push_back(new EnemyStateAttackBite);
	mStatePools.push_back(new EnemyStateDeath);

	// 当たり判定のセット
	AABB box = mesh->GetCollisionBox();
	box.Scaling(0.5f, 0.5f, 0.5f);
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
	// ステート外部からステート変更があったか
	if (mNowState != mNextState)
	{
		mStatePools[static_cast<unsigned int>(mNextState)]->Enter(this, deltaTime);
		mNowState = mNextState;
		return;
	}

	// ステート実行
	mNextState = mStatePools[static_cast<unsigned int>(mNowState)]->Update(this, deltaTime);

	// ステート内部から変更があったか
	if (mNowState != mNextState)
	{
		mStatePools[static_cast<unsigned int>(mNextState)]->Enter(this, deltaTime);
		mNowState = mNextState;
	}

	// いずれかの移動処理で得た速度で現在位置更新
	mPosition += mVelocity;
	SetComputeWorldTransform();
}

void EnemyObject::OnCollisionEnter(ColliderComponent* ownCollider, ColliderComponent* otherCollider)
{
}
