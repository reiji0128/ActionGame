#include "EnemyStateAttackFireBall.h"
#include "Game.h"
#include "AudioManager.h"
#include "SkeletalMeshComponent.h"
#include "AttackObj.h"
#include "FireBallObject.h"

EnemyStateAttackFireBall::EnemyStateAttackFireBall()
{
}

EnemyStateAttackFireBall::~EnemyStateAttackFireBall()
{
}

EnemyState EnemyStateAttackFireBall::Update(EnemyObject* owner, float deltaTime)
{
	mStateTimer -= deltaTime;

	if (owner->GetHP() <= 0)
	{
		return EnemyState::STATE_DEATH;
	}

	RotateProcess(owner, deltaTime);

	if (mStateTimer <= 0.0f)
	{
		AttackProcess(owner, deltaTime);

		// �^�C�}�[�𖳌���Ƀ��Z�b�g���čU�����J��Ԃ��Ȃ��悤�ɂ���
		mStateTimer = FLT_MAX;
	}

	// �G����v���C���[�Ɍ������x�N�g�����Z�o
	Vector3 playerPos = GAMEINSTANCE.GetPlayerObjects()->GetPosition();
	Vector3 enemyToPlayerVec = playerPos - owner->GetPosition();

	float length = enemyToPlayerVec.Length();

	// �A�j���[�V�����̍Đ��͏I�������
	if (!mSkelComp->IsPlaying())
	{
		// �v���C���[�Ƃ̋������ǐՂ��鋗�������Z����
		if (length < mNearLength)
		{
			return EnemyState::STATE_IDLE;
		}
		else
		{
			// �������Z�o
			int probability = rand() % 100;

			// 40%�̊m���łЂ������U���ڍs
			if (0 <= probability && probability < 60)
			{
				return EnemyState::STATE_WALK;
			}

			if (70 <= probability && probability < 100)
			{
				return EnemyState::STATE_RUN;
			}
		}
	}

	return EnemyState::STATE_FIRE_BALL;
}

void EnemyStateAttackFireBall::Enter(EnemyObject* owner, float deltaTime)
{
	// �A�j���[�V�����̍Đ�
	mSkelComp = owner->GetSkeletalMeshComp();
	mSkelComp->PlayAnimation(owner->GetAnim(EnemyState::STATE_FIRE_BALL),0.5f);

	// �X�e�[�g�^�C�}�[�̏�����
	mStateTimer = 1.0f;

	// �v���C���[���W�̎擾
	Vector3 playerPos = GAMEINSTANCE.GetPlayerObjects()->GetPosition();
	// �G�̍��W�̎擾
	Vector3 ownerPos = owner->GetPosition();
	// �G����v���C���[�Ɍ������x�N�g���̒������Z�o
	mEnemyToPlayerVec = playerPos - ownerPos;
	float length = mEnemyToPlayerVec.Length();
	mEnemyToPlayerVec.z = 0.0f;
	mEnemyToPlayerVec.Normalize();

	Vector3 velosity = Vector3(0, 0, 0);
	owner->SetVelosity(velosity);

	// �v���C���[���W�̎擾
	playerPos = GAMEINSTANCE.GetPlayerObjects()->GetPosition();
	// �G�̍��W�̎擾
	ownerPos = owner->GetPosition();
	// �G����v���C���[�Ɍ������x�N�g�����Z�o
	mAimVec = playerPos - ownerPos;
	mAimVec.z = 0.0f;
	mAimVec.Normalize();

	mRotateNow = true;

	//owner->ChangeLightColor(Vector3(1, 0, 0));
}

void EnemyStateAttackFireBall::AttackProcess(EnemyObject* owner, float deltaTime)
{
	Matrix4 animMatrix;

	// ���̍s���Vector3�ɕϊ�
	animMatrix =  owner->FindBoneMatrixFromName("MOUNTAIN_DRAGON_ Head");
	mMouthPos = Vector3::Transform(Vector3::Zero, animMatrix);
	mMouthPos.z += 250;

	new FireBallObject(mMouthPos, owner->GetDirection(), 500, Tag::EnemyFireBall);
	AUDIO->PlaySoundW("Assets/Sound/FireBall.wav");

}
