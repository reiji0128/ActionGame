#include "EnemyStatePatrol.h"
#include "EnemyObject.h"
#include "SkeletalMeshComponent.h"
#include "Game.h"

EnemyStatePatrol::EnemyStatePatrol()
	:mEnemyToPlayerVec(0,0,0)
{
}

EnemyStatePatrol::~EnemyStatePatrol()
{
}

EnemyState EnemyStatePatrol::Update(EnemyObject* owner, float deltaTime)
{
	Vector3 dir = owner->GetDirection();

	//��]����?
	if (mRotateNow)
	{
		// �ڕW�p�x�ɋ߂Â������ړ]�I���E���i��
		if (isNearAngle(dir, mAimVec))
		{
			dir = mAimVec;
			mRotateNow = false;
			owner->SetDirection(dir);
			owner->SetVelosity(dir * mPatrolSpeed * deltaTime);
			return EnemyState::STATE_PATROL;
		}
		// ��]
		dir = (zRotateToAimVec(dir, mAimVec, mRotateSpeed * deltaTime));
		owner->SetDirection(dir);
		owner->RotateToNewForward();
	}

	// �G�L�����̎��E&�͈͂ɓ�������ǔ����[�h��
	Vector3 playerPos = GAMEINSTANCE.GetPlayerObjects()->GetPosition();
	Vector3 ownerPos = owner->GetPosition();
	mEnemyToPlayerVec = playerPos - ownerPos;
	float length = mEnemyToPlayerVec.Length();


	if (InViewAngle(dir, mEnemyToPlayerVec, length))
	{
		return EnemyState::STATE_RUN;
	}

	owner->SetDirection(dir);

	// �O�i
	owner->SetVelosity(dir * mPatrolSpeed * deltaTime);

	return EnemyState::STATE_PATROL;
}

void EnemyStatePatrol::Enter(EnemyObject* owner, float deltaTime)
{
	// �A�C�h����Ԃ̃A�j���[�V�������Đ�
	mSkelComp = owner->GetSkeletalMeshComp();
	mSkelComp->PlayAnimation(owner->GetAnim(EnemyState::STATE_RUN),0.2f);

	mRotateNow = false;
}
