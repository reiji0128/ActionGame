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

	// ���b�V���̓ǂݍ��݁E�Z�b�g
	Mesh* mesh = GraphicResourceManager::LoadMesh(gpmeshFileName);
	mSkeltalMeshComp = new SkeletalMeshComponent(this, mUseShaderTag);
	mSkeltalMeshComp->SetMesh(mesh);

	// �X�P���g���̓ǂݍ���
	mSkeltalMeshComp->SetSkeleton(GraphicResourceManager::LoadSkeleton(gpskelFileName));

	// �A�j���[�V�����̓ǂݍ���
	mAnimations.resize(static_cast<unsigned int>(EnemyState::STATE_NUM));
	mAnimations[static_cast<unsigned int>(EnemyState::STATE_IDLE)]        = GraphicResourceManager::LoadAnimation("Assets/Dragon/DRAGON_idle.gpanim"                     , true);
	mAnimations[static_cast<unsigned int>(EnemyState::STATE_PATROL)]      = GraphicResourceManager::LoadAnimation("Assets/Dragon/ANIM_DRAGON_walk.gpanim"                , true);
	mAnimations[static_cast<unsigned int>(EnemyState::STATE_RUN)]         = GraphicResourceManager::LoadAnimation("Assets/Dragon/ANIM_DRAGON_walk.gpanim"                , true);
	mAnimations[static_cast<unsigned int>(EnemyState::STATE_ATTACK_HAND)] = GraphicResourceManager::LoadAnimation("Assets/Dragon/ANIM_MOUNTAIN_DRAGON_AttackHand.gpanim" , false);
	mAnimations[static_cast<unsigned int>(EnemyState::STATE_ATTACK_BITE)] = GraphicResourceManager::LoadAnimation("Assets/Dragon/ANIM_DRAGON_bite.gpanim"                , false);
	mAnimations[static_cast<unsigned int>(EnemyState::STATE_FIRE_BREATH)] = GraphicResourceManager::LoadAnimation("Assets/Dragon/ANIM_DRAGON_spreadFire.gpanim"          , false);
	mAnimations[static_cast<unsigned int>(EnemyState::STATE_FIRE_BALL)]   = GraphicResourceManager::LoadAnimation("Assets/Dragon/STATE_FIRE_BREATH.gpanim"               , false);
	mAnimations[static_cast<unsigned int>(EnemyState::STATE_DEATH)]       = GraphicResourceManager::LoadAnimation("Assets/Dragon/ANIM_DRAGON_death.gpanim"               , false);


	// �A�C�h����Ԃ̃A�j���[�V�������Z�b�g
	mSkeltalMeshComp->PlayAnimation(mAnimations[static_cast<unsigned int>(EnemyState::STATE_IDLE)]);

	// �A�N�^�[�X�e�[�g�v�[���̏�����
	mStatePools.push_back(new EnemyStateIdle);
	mStatePools.push_back(new EnemyStatePatrol);
	mStatePools.push_back(new EnemyStateRun);
	mStatePools.push_back(new EnemyStateAttackHand);
	mStatePools.push_back(new EnemyStateAttackBite);
	mStatePools.push_back(new EnemyStateDeath);

	// �����蔻��̃Z�b�g
	AABB box = mesh->GetCollisionBox();
	box.Scaling(0.5f, 0.5f, 0.5f);
	mBoxCollider = new BoxCollider(this);
	mBoxCollider->SetObjectBox(box);

	// ���C���R���C�_�[�̃Z�b�g
	mLineCollider = new LineCollider(this);
	Line line(Vector3(0, 0, 50.0f), Vector3(0, 0, -20.0f));
	mLineCollider->SetLine(line);
}

EnemyObject::~EnemyObject()
{
}

void EnemyObject::UpdateGameObject(float deltaTime)
{
	// �X�e�[�g�O������X�e�[�g�ύX����������
	if (mNowState != mNextState)
	{
		mStatePools[static_cast<unsigned int>(mNextState)]->Enter(this, deltaTime);
		mNowState = mNextState;
		return;
	}

	// �X�e�[�g���s
	mNextState = mStatePools[static_cast<unsigned int>(mNowState)]->Update(this, deltaTime);

	// �X�e�[�g��������ύX����������
	if (mNowState != mNextState)
	{
		mStatePools[static_cast<unsigned int>(mNextState)]->Enter(this, deltaTime);
		mNowState = mNextState;
	}

	// �����ꂩ�̈ړ������œ������x�Ō��݈ʒu�X�V
	mPosition += mVelocity;
	SetComputeWorldTransform();
}

void EnemyObject::OnCollisionEnter(ColliderComponent* ownCollider, ColliderComponent* otherCollider)
{
}
