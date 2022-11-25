#include "EnemyObject.h"
#include "Game.h"
#include "Renderer.h"
#include "GraphicResourceManager.h"
#include "DamageNotificator.h"
#include "Mesh.h"
#include "Skeleton.h"
#include "SkeletalMeshComponent.h"
#include "BoxCollider.h"
#include "LineCollider.h"
#include "Animation.h"
#include "PointLightComponent.h"
#include "HitEffect.h"
#include "ShaderTag.h"
#include "FireBreathObject.h"

#include "EnemyStaeBase.h"
#include "EnemyStateDetection.h"
#include "EnemyStateIdle.h"
#include "EnemyStatePatrol.h"
#include "EnemyStateWalk.h"
#include "EnemyStateRun.h"
#include "EnemyStatePreliminaryActionHand.h"
#include "EnemyStatePreliminaryActionBite.h"
#include "EnemyStateAttackHand.h"
#include "EnemyStateAttackBite.h"
#include "EnemyStateAttackFireBreath.h"
#include "EnemyStateAttackFireBall.h"
#include "EnemyStateDamage.h"
#include "EnemyStateDeath.h"

EnemyObject::EnemyObject(const Vector3& pos,const float& scale,const char* gpmeshFileName,const char* gpskelFileName)
	:GameObject(Tag::Enemy)
	,mEnvincibleTime(3.0f)
	,mEnvincibleTimer(0.0f)
	,mStartEnvincibleTimer(false)
	,mIsEnvincible(false)
	,mSkeltalMeshComp(nullptr)
	,mBoxCollider(nullptr)
	,mLineCollider(nullptr)
	,mHitEffect(nullptr)
	,mNowState()
	,mNextState()
	,mIsBackGroundTrigger(false)
	,mUseShaderTag(ShaderTag::SKINNED_SHADOW_MAP)
{
	SetPosition(pos);

	SetScale(scale);

	SetDirection(Vector3(-1, 0, 0));

	mHP = mMaxHP;

	mNowState = EnemyState::STATE_NUM;

	// ���b�V���̓ǂݍ��݁E�Z�b�g
	Mesh* mesh = GraphicResourceManager::LoadMesh(gpmeshFileName);
	mSkeltalMeshComp = new SkeletalMeshComponent(this, mUseShaderTag);
	mSkeltalMeshComp->SetMesh(mesh);

	// �X�P���g���̓ǂݍ���
	mSkeltalMeshComp->SetSkeleton(GraphicResourceManager::LoadSkeleton(gpskelFileName));

	// �A�j���[�V�����̓ǂݍ���
	mAnimations.resize(static_cast<unsigned int>(EnemyState::STATE_NUM));
	mAnimations[static_cast<unsigned int>(EnemyState::STATE_DETECTION)]   = GraphicResourceManager::LoadAnimation("Assets/Dragon/ANIM_DRAGON_walk.gpanim"                  , true);
	mAnimations[static_cast<unsigned int>(EnemyState::STATE_IDLE)]        = GraphicResourceManager::LoadAnimation("Assets/Dragon/ANIM_DRAGON_walk.gpanim" , true);
	mAnimations[static_cast<unsigned int>(EnemyState::STATE_PATROL)]      = GraphicResourceManager::LoadAnimation("Assets/Dragon/ANIM_DRAGON_walk.gpanim"                 , true);
	mAnimations[static_cast<unsigned int>(EnemyState::STATE_WALK)]        = GraphicResourceManager::LoadAnimation("Assets/Dragon/ANIM_DRAGON_walk.gpanim"                 , true);
	mAnimations[static_cast<unsigned int>(EnemyState::STATE_RUN)]         = GraphicResourceManager::LoadAnimation("Assets/Dragon/ANIM_DRAGON_run.gpanim"                  , true);
	mAnimations[static_cast<unsigned int>(EnemyState::STATE_PRELIMINARY_ACTION_HAND)] = GraphicResourceManager::LoadAnimation("Assets/Dragon/ANIM_MOUNTAIN_PreliminaryActionHand.gpanim", false);
	mAnimations[static_cast<unsigned int>(EnemyState::STATE_PRELIMINARY_ACTION_BITE)] = GraphicResourceManager::LoadAnimation("Assets/Dragon/ANIM_MOUNTAIN_PreliminaryActionBite.gpanim", false);
	mAnimations[static_cast<unsigned int>(EnemyState::STATE_ATTACK_HAND)] = GraphicResourceManager::LoadAnimation("Assets/Dragon/ANIM_MOUNTAIN_DRAGON_AttackHand0.gpanim"  , false);
	mAnimations[static_cast<unsigned int>(EnemyState::STATE_ATTACK_BITE)] = GraphicResourceManager::LoadAnimation("Assets/Dragon/ANIM_DRAGON_bite.gpanim"                 , false);
	mAnimations[static_cast<unsigned int>(EnemyState::STATE_FIRE_BREATH)] = GraphicResourceManager::LoadAnimation("Assets/Dragon/ANIM_DRAGON_spreadFire.gpanim"           , false);
	mAnimations[static_cast<unsigned int>(EnemyState::STATE_FIRE_BALL)]   = GraphicResourceManager::LoadAnimation("Assets/Dragon/ANIM_DRAGON_spitFireBall.gpanim"         , false);
	mAnimations[static_cast<unsigned int>(EnemyState::STATE_DAMAGE)]      = GraphicResourceManager::LoadAnimation("Assets/Dragon/ANIM_MOUNTAIN_DRAGON_Damage.gpanim"      , false);
	mAnimations[static_cast<unsigned int>(EnemyState::STATE_DEATH)]       = GraphicResourceManager::LoadAnimation("Assets/Dragon/ANIM_DRAGON_death.gpanim"                , false);

	// �A�N�^�[�X�e�[�g�v�[���̏�����
	mStatePools.push_back(new EnemyStateDetection);
	mStatePools.push_back(new EnemyStateIdle);
	mStatePools.push_back(new EnemyStatePatrol);
	mStatePools.push_back(new EnemyStateWalk);
	mStatePools.push_back(new EnemyStateRun);
	mStatePools.push_back(new EnemyStatePreliminaryActionHand);
	mStatePools.push_back(new EnemyStatePreliminaryActionBite);
	mStatePools.push_back(new EnemyStateAttackHand);
	mStatePools.push_back(new EnemyStateAttackBite);
	mStatePools.push_back(new EnemyStateAttackFireBreath);
	mStatePools.push_back(new EnemyStateAttackFireBall);
	mStatePools.push_back(new EnemyStateDamage);
	mStatePools.push_back(new EnemyStateDeath);

	// ���̃X�e�[�g�̐ݒ�
	mNextState = EnemyState::STATE_DETECTION;

	// �A�C�h����Ԃ̃A�j���[�V�������Z�b�g
	mSkeltalMeshComp->PlayAnimation(mAnimations[static_cast<unsigned int>(EnemyState::STATE_DETECTION)]);

	// �����蔻��̃Z�b�g
	AABB box = mesh->GetCollisionBox();
	box.Scaling(0.2f, 0.3f, 0.3f);
	box.Offset(Vector3(0,0,0));
	box.mMin.y += 200;
	box.mMin.z += 400;
	box.mIsRotatable = false;
	mBoxCollider = new BoxCollider(this);
	mBoxCollider->SetObjectBox(box);

	//// �w�i�̓����蔻��p�g���K�[�̃Z�b�g
	//AABB enemyForwardBox;
	//enemyForwardBox.mMin.x = box.mMax.x;
	//enemyForwardBox.mMin.y = box.mMin.y;
	//enemyForwardBox.mMin.z = 100.0f;
	//enemyForwardBox.mMax.x = enemyForwardBox.mMin.x + 300.0f;
	//enemyForwardBox.mMax.y = enemyForwardBox.mMin.y + 300.0f;
	//enemyForwardBox.mMax.z = 150.0f;
	//enemyForwardBox.mIsRotatable = true;
	//mBackGroundTrigger = new BoxCollider(this);
	//mBackGroundTrigger->SetObjectBox(enemyForwardBox);

	// ���C���R���C�_�[�̃Z�b�g
	mLineCollider = new LineCollider(this);
	Line line(Vector3(0, 0, 50.0f), Vector3(0, 0, -20.0f));
	mLineCollider->SetLine(line);

	mHitEffect = new HitEffect(this,Vector3(0,0,100));

	// �|�C���g���C�g�̃Z�b�g
	//mCenterLight     = new PointLightComponent(this, Vector3(   0, 100, 0), Vector3(1.0f, 0.55f, 0.0f), 15.0f);
	/*mBehindLight     = new PointLightComponent(this, Vector3( 100, 100, 10), Vector3(0.1f, 0.0f, 0.0f), 5.0f);
	mFrontLight      = new PointLightComponent(this, Vector3(-100, 100, 10), Vector3(0.1f, 0.0f, 0.0f), 5.0f);
	mLeftLight       = new PointLightComponent(this, Vector3(   0, 200, 10), Vector3(0.1f, 0.0f, 0.0f), 5.0f);
	mRightLight      = new PointLightComponent(this, Vector3(   0,   0, 10), Vector3(0.1f, 0.0f, 0.0f), 5.0f);
	mFrontLeftLight  = new PointLightComponent(this, Vector3(-100, 200, 10), Vector3(0.1f, 0.0f, 0.0f), 5.0f);
    mFrontRightLight = new PointLightComponent(this, Vector3(-100,   0, 10), Vector3(0.1f, 0.0f, 0.0f), 5.0f);*/

	mAttackList[DamageType::EnemyHandAttack] = 10;
	mAttackList[DamageType::EnemyBiteAttack] = 20;
	mAttackList[DamageType::EnemyFireBreath] = 10;
	mAttackList[DamageType::EnemyFireBall]   = 30;

	DamageNotificator::AddDamageFunction(DamageType::EnemyHandAttack, std::bind(&EnemyObject::GetDamage, this, std::placeholders::_1));
	DamageNotificator::AddDamageFunction(DamageType::EnemyBiteAttack, std::bind(&EnemyObject::GetDamage, this, std::placeholders::_1));
	DamageNotificator::AddDamageFunction(DamageType::EnemyFireBreath, std::bind(&EnemyObject::GetDamage, this, std::placeholders::_1));
	DamageNotificator::AddDamageFunction(DamageType::EnemyFireBall  , std::bind(&EnemyObject::GetDamage, this, std::placeholders::_1));
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

	mIsBackGroundTrigger = false;

	if (mIsDamage && !mIsEnvincible)
	{
		Damage(10);
		mHitEffect->PlayEffect();

		mStartEnvincibleTimer = true;
		mEnvincibleTimer = mEnvincibleTime;
	}

	// ���G���Ԃ̃^�C�}�[���n�܂��Ă�����
	if (mStartEnvincibleTimer)
	{
		mEnvincibleTimer -= deltaTime;

		if (mEnvincibleTimer <= 0)
		{
			mIsEnvincible = false;
		}
		else
		{
			mIsEnvincible = true;
		}
	}
}

void EnemyObject::OnCollisionEnter(ColliderComponent* ownCollider, ColliderComponent* otherCollider)
{
	Tag colliderTag = otherCollider->GetTag();

	//�Փˏ��
	CollisionInfo info;

	// �w�i�ƏՓ˂���?
	if (colliderTag == Tag::BackGround)
	{
		// �Փ˔w�i��Box?
		if (otherCollider->GetColliderType() == ColliderTypeEnum::Box)
		{
			// �w�iBox�ɏՓ˂����̂�Line?
			if (ownCollider == mLineCollider)
			{
				//�������Ă���Ƃ��̂ݔ���
				if (mVelocity.z < 0.0f)
				{
					info = ownCollider->GetCollisionInfo();
					mPosition = info.mCollisionPoint;
				}
			}

			// �w�i�p�g���K�[
			if (ownCollider == mBackGroundTrigger)
			{
				mIsBackGroundTrigger = true;
			}

			// �����R���C�_�[
			if (ownCollider == mLineCollider)
			{
				info = ownCollider->GetCollisionInfo();
				mPosition = info.mCollisionPoint;
				ComputeWorldTransform();
			}

			// �w�iBox�ɏՓ˂����̂�Box�H
			if (ownCollider->GetColliderType() == ColliderTypeEnum::Box)
			{
				//�q�b�g�{�b�N�X�H
				if (ownCollider == mBoxCollider)
				{
					info = ownCollider->GetCollisionInfo();

					mPosition += info.mFixVec;
					ComputeWorldTransform();
				}
			}
		}
	}

	// �v���C���[�ƂԂ�������
	if (colliderTag == Tag::Player)
	{
		if (ownCollider == mBoxCollider)
		{
			info = ownCollider->GetCollisionInfo();
			mPosition += info.mFixVec;
			ComputeWorldTransform();
		}
	}

	if (colliderTag == Tag::PlayerFirstAttack)
	{
		mIsDamage = true;
		mDamage = DamageNotificator::Norificate(DamageType::PlayerFirstAttack);
	}

	if (colliderTag == Tag::PlayerSecondAttack)
	{
		mIsDamage = true;
		mDamage = DamageNotificator::Norificate(DamageType::PlayerSecondAttack);
	}
}

void EnemyObject::Damage(int damage)
{
	mHP = mHP - damage;

	if (mHP <= 0)
	{
		mHP = 0;
	}

	mIsDamage = false;
}

Matrix4 EnemyObject::FindBoneMatrixFromName(const char* boneName)
{
	int boneIndex;
	Matrix4 animMatrix;

	// �{�[���̖��O����Ή�����C���f�b�N�X���擾
	boneIndex = mSkeltalMeshComp->CallGetBoneIndexFromName(boneName);

	// �C���f�b�N�X����s����擾
	animMatrix = mSkeltalMeshComp->GetBoneMatrix(boneIndex);

	return animMatrix;
}

void EnemyObject::ChangeLightColor(Vector3 color)
{
	mCenterLight->SetColor(color);
	mBehindLight->SetColor(color);
	mFrontLight->SetColor(color);
	mLeftLight->SetColor(color);
	mRightLight->SetColor(color);
	mFrontLeftLight->SetColor(color);
	mFrontRightLight->SetColor(color);
}

int EnemyObject::GetDamage(DamageType type) const
{
	// �_���[�W�̎�ނ�T��
	auto iter = mAttackList.find(type);

	// ����������Ή�����_���[�W��Ԃ�
	if (iter != mAttackList.end())
	{
		auto list = *iter;
		return list.second;
	}
}
