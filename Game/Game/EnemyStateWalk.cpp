#include "EnemyStateWalk.h"
#include "Game.h"
#include "SkeletalMeshComponent.h"

EnemyStateWalk::EnemyStateWalk()
	:mWalkSpeed(200.0f)
{
}

EnemyStateWalk::~EnemyStateWalk()
{
}

EnemyState EnemyStateWalk::Update(EnemyObject* owner, float deltaTime)
{
	mStateTimer -= deltaTime;

	if (owner->GetHP() <= 0)
	{
		return EnemyState::STATE_DEATH;
	}

	// �v���C���[���W�̎擾
	Vector3 playerPos = GAMEINSTANCE.GetPlayerObjects()->GetPosition();
	// �G�̍��W�̎擾
	Vector3 ownerPos = owner->GetPosition();
	// �G����v���C���[�Ɍ������x�N�g���̒������Z�o
	mEnemyToPlayerVec = playerPos - ownerPos;
	float length = mEnemyToPlayerVec.Length();
	mEnemyToPlayerVec.Normalize();

	// �G�̌����̎擾
	Vector3 dir = owner->GetDirection();

	//�v���C���[�Ƃ̋������߂��ꍇ
	if (length < mFarLength && mStateTimer < 0.0)
	{
		// �������Z�o
		int probability = rand() % 100;

		// 40%�̊m���łЂ������U���ڍs
		if (0 <= probability && probability < 40)
		{
			return EnemyState::STATE_PRELIMINARY_ACTION_HAND;
		}

		// 40%�̊m���Ŋ��݂��U���Ɉڍs
		if (40 <= probability && probability < 80)
		{
			return EnemyState::STATE_PRELIMINARY_ACTION_BITE;
		}

		// 20%�̊m���Ń�����ԂɈڍs
		if (80 <= probability && probability < 100)
		{
			return EnemyState::STATE_RUN;
		}
	}

	// �v���C���[�Ƃ̋����������ꍇ
	if (length > mNearLength && mStateTimer < 0.0)
	{
		// �������Z�o
		int probability = rand() % 100;

		// 30%�̊m���Ń�����ԂɈڍs
		if (0 <= probability && probability < 30)
		{
			return EnemyState::STATE_RUN;
		}

		// 35%�̊m���Ńu���X�U���Ɉڍs
		if (30 <= probability && probability < 65)
		{
			return EnemyState::STATE_FIRE_BREATH;
		}

		// 35%�̊m���ŉ΋��U���Ɉڍs
		if (65 <= probability && probability < 100)
		{
			return EnemyState::STATE_FIRE_BALL;
		}
	}

	// �G����v���C���[�Ɍ������x�N�g����G�̌����ɃZ�b�g
	mEnemyToPlayerVec.z = 0.0f;
	mEnemyToPlayerVec.Normalize();
	owner->SetDirection(mEnemyToPlayerVec);

	//�ړ�����
	MoveCalc(owner, deltaTime);

	return EnemyState::STATE_WALK;
}

void EnemyStateWalk::Enter(EnemyObject* owner, float deltaTime)
{
	// �A�j���[�V�����̍Đ�
	mSkelComp = owner->GetSkeletalMeshComp();
	mSkelComp->PlayAnimation(owner->GetAnim(EnemyState::STATE_WALK),0.4f);

	mStateTimer = 2.0f;
}

void EnemyStateWalk::MoveCalc(EnemyObject* owner, float deltaTime)
{
		// �v���C���[�Ɍ������x�N�g��
		Vector3 dir = mEnemyToPlayerVec;
		dir.z = 0.0f;
		dir.Normalize();

		Vector3 velocity = dir * mWalkSpeed * deltaTime;
		owner->SetVelosity(velocity);

		owner->SetDirection(dir);
		owner->SetComputeWorldTransform();
}
