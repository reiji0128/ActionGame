#include "EnemyStateRun.h"
#include "SkeletalMeshComponent.h"
#include "Game.h"

EnemyStateRun::EnemyStateRun()
{
}

EnemyState EnemyStateRun::Update(EnemyObject* owner, float deltaTime)
{
	// �v���C���[�ʒu���
	Vector3 playerPos = GAMEINSTANCE.GetPlayerObjects()->GetPosition();
	Vector3 pos = owner->GetPosition();
	enemyToPlayerVec = playerPos - pos;

	float length = enemyToPlayerVec.Length();
	enemyToPlayerVec.Normalize();

	//�ǐՂ��Ȃ������܂ŋ߂Â����Ƃ��ɍU���Ɉڍs
	if (length < nearLength)
	{
		return EnemyState::STATE_ATTACK_HAND;
	}

	// �ǐՉ\�����𒴂�����X�e�[�g���p�g���[���Ɉڍs
	if (length > trackingRange)
	{
		return EnemyState::STATE_PATROL;
	}

	//�ړ�����
	MoveCalc(owner, deltaTime);
	return EnemyState::STATE_RUN;
}

void EnemyStateRun::Enter(EnemyObject* owner, float deltaTime)
{
	mSkelComp = owner->GetSkeletalMeshComp();
	mSkelComp->PlayAnimation(owner->GetAnim(EnemyState::STATE_RUN),0.3f);
}

void EnemyStateRun::MoveCalc(EnemyObject* owner, float deltaTime)
{
	//���������̌������擾
	Vector3 dir = enemyToPlayerVec;
	dir.z = 0.0f;
	dir.Normalize();

	Vector3 velocity = dir * runSpeed * deltaTime;
	owner->SetVelosity(velocity);

	owner->SetDirection(dir);
	owner->SetComputeWorldTransform();
}
