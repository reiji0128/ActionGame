#include "EnemyStateIdle.h"
#include "EnemyObject.h"
#include "SkeletalMeshComponent.h"
#include "Game.h"

EnemyStateIdle::EnemyStateIdle()
{
}

EnemyStateIdle::~EnemyStateIdle()
{
}


EnemyState EnemyStateIdle::Update(EnemyObject* owner, float deltaTime)
{
	mStateTimer -= deltaTime;

	// �v���C���[�ʒu���
	Vector3 playerPos = GAMEINSTANCE.GetPlayerObjects()->GetPosition();
	Vector3 mPosition = owner->GetPosition();
	Vector3 enemyToPlayerVec = playerPos - mPosition;

	float length = enemyToPlayerVec.Length();

	//�ǐՂ��Ȃ������܂ŋ߂Â����Ƃ��ɍU���Ɉڍs
	if (length < mNearLength)
	{
		return EnemyState::STATE_ATTACK_HAND;
	}

	if (mStateTimer < 0.0f)
	{
		return EnemyState::STATE_PATROL;
	}

	return EnemyState::STATE_IDLE;
}

void EnemyStateIdle::Enter(EnemyObject* owner, float deltaTime)
{
	// �A�C�h����Ԃ̃A�j���[�V�������Đ�
	mSkelComp = owner->GetSkeletalMeshComp();
	mSkelComp->PlayAnimation(owner->GetAnim(EnemyState::STATE_IDLE));
}
