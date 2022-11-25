#include "EnemyStateAttackBite.h"
#include "EnemyObject.h"
#include "Game.h"
#include "SkeletalMeshComponent.h"
#include "AttackObj.h"

EnemyStateAttackBite::EnemyStateAttackBite()
{
}

EnemyStateAttackBite::~EnemyStateAttackBite()
{
}

EnemyState EnemyStateAttackBite::Update(EnemyObject* owner, float deltaTime)
{
	mStateTimer -= deltaTime;

	if (owner->GetHP() <= 0)
	{
		return EnemyState::STATE_DEATH;
	}

	RotateProcess(owner, deltaTime);

	// �G����v���C���[�Ɍ������x�N�g�����Z�o
	Vector3 playerPos = GAMEINSTANCE.GetPlayerObjects()->GetPosition();
	Vector3 enemyToPlayerVec = playerPos - owner->GetPosition();

	float length = enemyToPlayerVec.Length();

	mAimVec = enemyToPlayerVec;
	mAimVec.z = 0.0f;
	mAimVec.Normalize();

	// �A�j���[�V�����͍Đ�����
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

			// 30%�̊m���łЂ������U���ڍs
			if (0 <= probability && probability < 40)
			{
				return EnemyState::STATE_WALK;
			}

			if (40 <= probability && probability < 100)
			{
				return EnemyState::STATE_RUN;
			}
		}
	}

	// �U������
	Attack(owner, deltaTime);

	enemyToPlayerVec.z = 0.0f;
	enemyToPlayerVec.Normalize();

	return EnemyState::STATE_ATTACK_BITE;
}

void EnemyStateAttackBite::Enter(EnemyObject* owner, float deltaTime)
{
	// �A�j���[�V�����̍Đ�
	mSkelComp = owner->GetSkeletalMeshComp();
	mSkelComp->PlayAnimation(owner->GetAnim(EnemyState::STATE_ATTACK_BITE),0.3f);

	mStateTimer = attackOffsetTime;

	Vector3 velosity = velosity.Zero;
	owner->SetVelosity(velosity);

	// �v���C���[���W�̎擾
	Vector3 playerPos = GAMEINSTANCE.GetPlayerObjects()->GetPosition();

	// �G�̍��W�̎擾
	Vector3 ownerPos = owner->GetPosition();

	// �G����v���C���[�Ɍ������x�N�g�����Z�o
	mAimVec = playerPos - ownerPos;
	mAimVec.z = 0.0f;
	mAimVec.Normalize();

	mRotateNow = true;

	//owner->ChangeLightColor(Vector3(0, 1, 0));
}

void EnemyStateAttackBite::Attack(EnemyObject* owner, float deltaTime)
{
	Vector3 ownerPos = owner->GetPosition();
	mAttackObjOffset = Vector3(-100, 0, 0);
	Vector3 attackObjPos = ownerPos + mAttackObjOffset;

	if (mStateTimer < 0.0)
	{
		new AttackObj(Tag::EnemyAttackBite, owner->GetPosition(), Vector3(300, 300, 0), owner->GetDirection(), 300.0f, 1.0f);

		// �^�C�}�[�𖳌���Ƀ��Z�b�g���čU�����J��Ԃ��Ȃ��悤�ɂ���
		mStateTimer = FLT_MAX;
	}

}
