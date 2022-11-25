#include "EnemyStateRun.h"
#include "SkeletalMeshComponent.h"
#include "Game.h"
#include "AttackObj.h"

EnemyStateRun::EnemyStateRun()
	:runSpeed(500.0f)
{
}

EnemyState EnemyStateRun::Update(EnemyObject* owner, float deltaTime)
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
	Vector3 enemyToPlayerVec = playerPos - ownerPos;
	float length = enemyToPlayerVec.Length();
	enemyToPlayerVec.Normalize();

	// �G�̌����̎擾
	Vector3 dir = owner->GetDirection();

	if (!isNearAngle(owner->GetDirection(), enemyToPlayerVec))
	{
		mRotateNow = true;
	}

	RotateProcess(owner,deltaTime);
	if (!mRotateNow)
	{
		//�v���C���[�Ƃ̋������߂��ꍇ
		if (InViewAngle(dir, enemyToPlayerVec, length) || mStateTimer < 0.0)
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

			// 10%�̊m���ŉ΃u���X�U���Ɉڍs
			if (80 <= probability && probability < 90)
			{
				return EnemyState::STATE_FIRE_BREATH;
			}

			// 10%�̊m���ŉ΋��U���Ɉڍs
			if (90 <= probability && probability < 100)
			{
				return EnemyState::STATE_FIRE_BALL;
			}
		}

		// �v���C���[�Ƃ̋����������ꍇ
		if (length > mNearLength && mStateTimer < 0.0)
		{
			// �������Z�o
			int probability = rand() % 100;

			// 30%�̊m���łЂ������U���ڍs
			if (0 <= probability && probability < 20)
			{
				return EnemyState::STATE_WALK;
			}

			// 35%�̊m���Ńu���X�U���Ɉڍs
			if (20 <= probability && probability < 60)
			{
				return EnemyState::STATE_FIRE_BREATH;
			}

			// 35%�̊m���ŉ΋��U���Ɉڍs
			if (60 <= probability && probability < 100)
			{
				return EnemyState::STATE_FIRE_BALL;
			}
		}

		// �G����v���C���[�Ɍ������x�N�g����G�̌����ɃZ�b�g
		/*enemyToPlayerVec.z = 0.0f;
		enemyToPlayerVec.Normalize();
		owner->SetDirection(enemyToPlayerVec);*/

		//�ړ�����
		MoveCalc(owner, deltaTime);
	}

	return EnemyState::STATE_RUN;
}

void EnemyStateRun::Enter(EnemyObject* owner, float deltaTime)
{
	// �A�j���[�V�����̍Đ�
	mSkelComp = owner->GetSkeletalMeshComp();
	mSkelComp->PlayAnimation(owner->GetAnim(EnemyState::STATE_RUN),0.8f);
	Attack(owner, deltaTime);
	mStateTimer = 4.0f;

	// �v���C���[���W�̎擾
	Vector3 playerPos = GAMEINSTANCE.GetPlayerObjects()->GetPosition();
	// �G�̍��W�̎擾
	Vector3 ownerPos = owner->GetPosition();
	// �G����v���C���[�Ɍ������x�N�g���̒������Z�o
	mEnemyToPlayerVec = playerPos - ownerPos;
	float length = mEnemyToPlayerVec.Length();
	mEnemyToPlayerVec.z = 0.0f;
	mEnemyToPlayerVec.Normalize();
	//owner->SetDirection(mEnemyToPlayerVec);
	mPlayerPos = GAMEINSTANCE.GetPlayerObjects()->GetPosition();

	mRotateNow = true;
}

void EnemyStateRun::MoveCalc(EnemyObject* owner, float deltaTime)
{
	//���������̌������擾
	Vector3 dir = mEnemyToPlayerVec;
	dir.z = 0.0f;
	dir.Normalize();

	Vector3 velocity = dir * runSpeed * deltaTime;

	owner->SetVelosity(velocity);

	owner->SetDirection(dir);
	owner->SetComputeWorldTransform();
}

void EnemyStateRun::RotateProcess(EnemyObject* owner, float deltaTime)
{
		//���������̌������擾
		Vector3 dir = mEnemyToPlayerVec;
		dir.z = 0.0f;
		dir.Normalize();

		Vector3 nowDirection = owner->GetDirection();

		// �v���C���[���W�̎擾
		Vector3 playerPos = GAMEINSTANCE.GetPlayerObjects()->GetPosition();
		// �G�̍��W�̎擾
		Vector3 ownerPos = owner->GetPosition();
		// �G����v���C���[�Ɍ������x�N�g���̒������Z�o
		Vector3 aimDir = mPlayerPos - ownerPos;
		aimDir.Normalize();

		float rotateSpeed = 45.0f;

		if (mRotateNow)
		{
			if (isNearAngle(nowDirection, aimDir))
			{
				owner->SetDirection(nowDirection);
				mRotateNow = false;
				Vector3 velocity = nowDirection * rotateSpeed * deltaTime;
				owner->SetVelosity(velocity);
			}

			// ��]����
			nowDirection = zRotateToAimVec(nowDirection, aimDir, rotateSpeed * deltaTime);
			owner->SetDirection(nowDirection);
			owner->RotateToNewForward();
		}

		if (owner->GetIsBackGround())
		{
			// 90�x���v���ɕ����]������]���[�h�Ɉڍs
			mRotateNow = true;
			mAimVec = Vector3::Cross(Vector3::UnitZ, nowDirection);
			owner->SetVelosity(Vector3::Zero);
			return;
		}
}

void EnemyStateRun::Attack(EnemyObject* owner, float deltaTime)
{
	if (mStateTimer < 0.5)
	{
		new AttackObj(Tag::EnemyAttackBite, owner->GetPosition(), Vector3(500, 500, 0), owner->GetDirection(), 150.0f, mStateTimer);
		// �^�C�}�[�𖳌���Ƀ��Z�b�g���čU�����J��Ԃ��Ȃ��悤�ɂ���
		mStateTimer = FLT_MAX;
	}
}
