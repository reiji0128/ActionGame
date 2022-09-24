#include "PlayerObject.h"
#include "Game.h"
#include "Mesh.h"
#include "Skeleton.h"
#include "Renderer.h"
#include "GraphicResourceManager.h"
#include "BoxCollider.h"
#include "Animation.h"
#include "LineCollider.h"
#include "Input.h"
#include "ThirdPersonCameraObject.h"

#include "PlayerStateIdle.h"
#include "PlayerStateRun.h"
#include "PlayerStateFirstAttack.h"


PlayerObject::PlayerObject(const Vector3& pos, const float& scale, const char* gpmeshFileName, const char* gpskelFileName)
	:GameObject(Tag::Player)
	,mSkeltalMeshComp(nullptr)
	,mBoxCollider(nullptr)
	,mLineCollider(nullptr)
	,mBoneIndex(0)
	,mNowState()
	,mNextState()
	,mShaderTag(ShaderTag::SKINNED_SHADOW_MAP)
{
	// ���W�̃Z�b�g
	SetPosition(pos);

	// �X�P�[���̃Z�b�g
	SetScale(scale);

	SetDirection(Vector3(1, 0, 0));

	mHP = mMaxHP;

	// ���b�V���̓ǂݍ��݁E�Z�b�g
	Mesh* mesh = GraphicResourceManager::LoadMesh(gpmeshFileName);
	mSkeltalMeshComp = new SkeletalMeshComponent(this,mShaderTag);
	mSkeltalMeshComp->SetMesh(mesh);

	// �X�P���g���̓ǂݍ���
	mSkeltalMeshComp->SetSkeleton(GraphicResourceManager::LoadSkeleton(gpskelFileName));
	// �A�j���[�V�����̓ǂݍ���
	mAnimations.resize(static_cast<unsigned int>(PlayerState::STATE_NUM));
	mAnimations[static_cast<unsigned int>(PlayerState::STATE_IDLE)]         = GraphicResourceManager::LoadAnimation("Assets/Player/Great_Sword_Idle_Anim.gpanim"  , true);
	mAnimations[static_cast<unsigned int>(PlayerState::STATE_RUN)]          = GraphicResourceManager::LoadAnimation("Assets/Player/A_SW_Run.gpanim"               , true);
	mAnimations[static_cast<unsigned int>(PlayerState::STATE_FIRST_ATTACK)] = GraphicResourceManager::LoadAnimation("Assets/Player/Great_Sword_FirstSlash.gpanim" , true);
	
	// �A�C�h����Ԃ̃A�j���[�V�������Z�b�g
	mSkeltalMeshComp->PlayAnimation(mAnimations[static_cast<unsigned int>(PlayerState::STATE_IDLE)]);
	
	// �A�N�^�[�X�e�[�g�v�[���̏�����
	mStatePools.push_back(new PlayerStateIdle);
	mStatePools.push_back(new PlayerStateRun);
	mStatePools.push_back(new PlayerStateFirstAttack);

	// �����蔻��̃Z�b�g
	AABB box = mesh->GetCollisionBox();
	box.Scaling(1.0f,1.0f,1.0f);
	mBoxCollider = new BoxCollider(this);
	mBoxCollider->SetObjectBox(box);

	// ���C���R���C�_�[�̃Z�b�g
	mLineCollider = new LineCollider(this);
	Line line(Vector3(0, 0, 50.0f), Vector3(0, 0, -20.0f));
	mLineCollider->SetLine(line);
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
PlayerObject::~PlayerObject()
{
}

/// <summary>
/// �v���C���[�̍X�V����
/// </summary>
/// <param name="deltaTime">1�t���[���̌o�ߎ���</param>
void PlayerObject::UpdateGameObject(float deltaTime)
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
}

/// <summary>
/// �Փˏ���
/// </summary>
/// <param name="ownCollider"></param>
/// <param name="otherCollider"></param>
void PlayerObject::OnCollisionEnter(ColliderComponent* ownCollider, ColliderComponent* otherCollider)
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
					mIsOnGround = true;
				}
			}

			// �����R���C�_�[
			if (ownCollider == mLineCollider)
			{
				info = ownCollider->GetCollisionInfo();
				mPosition = info.mCollisionPoint;
				mIsOnGround = true;
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

	// �G�l�~�[�ƂԂ�������
	if (colliderTag == Tag::Enemy)
	{
		if (ownCollider == mBoxCollider)
		{
			info = ownCollider->GetCollisionInfo();
			mPosition += info.mFixVec;
			ComputeWorldTransform();
		}
	}
}
