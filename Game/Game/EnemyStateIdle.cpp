#include "EnemyStateIdle.h"
#include "EnemyObject.h"
#include "SkeletalMeshComponent.h"

EnemyStateIdle::EnemyStateIdle()
{
}


EnemyState EnemyStateIdle::Update(EnemyObject* owner, float deltaTime)
{
	return EnemyState::STATE_IDLE;
}

void EnemyStateIdle::Enter(EnemyObject* owner, float deltaTime)
{
	// �A�C�h����Ԃ̃A�j���[�V�������Đ�
	mSkelComp = owner->GetSkeletalMeshComp();
	mSkelComp->PlayAnimation(owner->GetAnim(EnemyState::STATE_IDLE));
}
