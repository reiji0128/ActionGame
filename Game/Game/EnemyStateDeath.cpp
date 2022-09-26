#include "EnemyStateDeath.h"
#include "EnemyObject.h"
#include "SkeletalMeshComponent.h"

EnemyStateDeath::EnemyStateDeath()
{
}

EnemyStateDeath::~EnemyStateDeath()
{
}

EnemyState EnemyStateDeath::Update(EnemyObject* owner, float deltaTime)
{
	if (!mSkelComp->IsPlaying())
	{
		owner->SetState(owner->EDead);
	}

	return EnemyState::STATE_DEATH;
}

void EnemyStateDeath::Enter(EnemyObject* owner, float deltaTime)
{
	// アイドル状態のアニメーションを再生
	mSkelComp = owner->GetSkeletalMeshComp();
	mSkelComp->PlayAnimation(owner->GetAnim(EnemyState::STATE_DEATH));
}
