#include "PlayerStateDeath.h"


PlayerStateDeath::PlayerStateDeath()
{
}

PlayerStateDeath::~PlayerStateDeath()
{
}

PlayerState PlayerStateDeath::Update(PlayerObject* owner, float deltaTime)
{
	//　アニメーションの再生が終わったら
	if (!mSkelComp->IsPlaying())
	{
		// プレイヤーをDead状態に
		owner->SetState(owner->EDead);
	}

	return PlayerState::STATE_DEATH;
}

void PlayerStateDeath::Enter(PlayerObject* owner, float deltaTime)
{
	mSkelComp = owner->GetSkeltalMeshComp();
	mSkelComp->PlayAnimation(owner->GetAnim(PlayerState::STATE_DEATH));
}
