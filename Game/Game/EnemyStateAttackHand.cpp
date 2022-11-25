#include "EnemyStateAttackHand.h"
#include "Game.h"
#include "SkeletalMeshComponent.h"
#include "AttackObj.h"

EnemyStateAttackHand::EnemyStateAttackHand()
	:mAttackOffsetTime(0.5f)
	,mMoveStopTime(3.2f)
	,mMoveStopTimer(0.0f)
{
}

EnemyStateAttackHand::~EnemyStateAttackHand()
{
}

EnemyState EnemyStateAttackHand::Update(EnemyObject* owner, float deltaTime)
{
	mStateTimer -= deltaTime;

	mMoveStopTimer += deltaTime;

	if (owner->GetHP() <= 0)
	{
		return EnemyState::STATE_DEATH;
	}

	// �G����v���C���[�Ɍ������x�N�g�����Z�o
	Vector3 playerPos = GAMEINSTANCE.GetPlayerObjects()->GetPosition();
	Vector3 enemyToPlayerVec = playerPos - owner->GetPosition();

	float length = enemyToPlayerVec.Length();
	RotateProcess(owner, deltaTime);

	mAimVec = enemyToPlayerVec;
	mAimVec.z = 0.0f;
	mAimVec.Normalize();

	if (mMoveStopTimer >= mMoveStopTime)
	{
		Vector3 velosity = Vector3(0, 0, 0);
		owner->SetVelosity(velosity);
	}

	// �A�j���[�V�����͍Đ�����
	if (!mSkelComp->IsPlaying())
	{
		// �v���C���[�Ƃ̋������ǐՋ������Z����
		if (length < mNearLength)
		{
			return EnemyState::STATE_IDLE;
		}
		else
		{
			// �������Z�o
			int probability = rand() % 100;

			// 30%�̊m���łЂ������U���ڍs
			if (0 <= probability && probability < 30)
			{
				return EnemyState::STATE_WALK;
			}

			if (70 <= probability && probability < 100)
			{
				return EnemyState::STATE_RUN;
			}
		}
	}

	Attack(owner,deltaTime);
	enemyToPlayerVec.z = 0.0f;
	enemyToPlayerVec.Normalize();
	//owner->SetDirection(enemyToPlayerVec);

	return EnemyState::STATE_ATTACK_HAND;
}

void EnemyStateAttackHand::Enter(EnemyObject* owner, float deltaTime)
{
	mStateTimer = mAttackOffsetTime;

	// �A�C�h����Ԃ̃A�j���[�V�������Đ�
	mSkelComp = owner->GetSkeletalMeshComp();
	mSkelComp->PlayAnimation(owner->GetAnim(EnemyState::STATE_ATTACK_HAND),0.3f);

	Vector3 velosity = Vector3(0,0,0);
	Vector3 dir = owner->GetDirection();
	dir.z = 0;
	dir.Normalize();

	velosity = dir * 100 * deltaTime;
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

	//owner->ChangeLightColor(Vector3(0, 0, 1));
}

void EnemyStateAttackHand::Attack(EnemyObject* owner, float deltaTime)
{
	//�U���\���ԂɂȂ�����U��
	if (mStateTimer < 0.0f)
	{
		new AttackObj(Tag::EnemyAttackHand, owner->GetPosition(), Vector3(200, 200, 0), owner->GetDirection(), 300.0f, 1.3f);
		mStateTimer = FLT_MAX;		// �^�C�}�[�𖳌���Ƀ��Z�b�g���čU�����J��Ԃ��Ȃ��悤�ɂ���
	}
}
