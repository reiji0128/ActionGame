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

	return EnemyState::STATE_ATTACK_BITE;
}

void EnemyStateAttackBite::Enter(EnemyObject* owner, float deltaTime)
{
	mSkelComp = owner->GetSkeletalMeshComp();
	mSkelComp->PlayAnimation(owner->GetAnim(EnemyState::STATE_ATTACK_BITE));

	// �U������
	Attack(owner, deltaTime);
}

void EnemyStateAttackBite::Attack(EnemyObject* owner, float deltaTime)
{
	if (mStateTimer < 0.0)
	{
		new AttackObj(Tag::EnemyAttackHand, owner->GetPosition(), owner->GetDirection(), 150.0f, 0.3f);

		// �^�C�}�[�𖳌���Ƀ��Z�b�g���čU�����J��Ԃ��Ȃ��悤�ɂ���
		mStateTimer = FLT_MAX;
	}

}
