#include "EnemyStateFireBreath.h"
#include "EnemyObject.h"
#include "Game.h"
#include "SkeletalMeshComponent.h"
#include "AttackObj.h"

EnemyStateFireBreath::EnemyStateFireBreath()
{
}

EnemyStateFireBreath::~EnemyStateFireBreath()
{
}

EnemyState EnemyStateFireBreath::Update(EnemyObject* owner, float deltaTime)
{
	// �G����v���C���[�Ɍ������x�N�g�����Z�o
	Vector3 playerPos = GAMEINSTANCE.GetPlayerObjects()->GetPosition();
	Vector3 enemyToPlayerVec = playerPos - owner->GetPosition();

	float length = enemyToPlayerVec.Length();

	// �A�j���[�V�����͍Đ�����
	if (!mSkelComp->IsPlaying())
	{
		// �v���C���[�Ƃ̋������ǐՂ��鋗�������Z����
		if (length < retrackingRange)
		{
			return EnemyState::STATE_IDLE;
		}
		else
		{
			return EnemyState::STATE_RUN;
		}
	}

	return EnemyState::STATE_FIRE_BREATH;
}

void EnemyStateFireBreath::Enter(EnemyObject* owner, float deltaTime)
{
	mSkelComp = owner->GetSkeletalMeshComp();
	mSkelComp->PlayAnimation(owner->GetAnim(EnemyState::STATE_FIRE_BREATH));
}

void EnemyStateFireBreath::AttackProcess(EnemyObject* owner, float deltaTime)
{
	if (mStateTimer < 0.0)
	{
		new AttackObj(Tag::EnemyFireBreath, owner->GetPosition(), owner->GetDirection(), 150.0f, 1.0f);
	}
}
