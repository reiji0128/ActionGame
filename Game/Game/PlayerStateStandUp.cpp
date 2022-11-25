#include "PlayerStateStandUp.h"

PlayerStateStandUp::PlayerStateStandUp()
{
}

PlayerStateStandUp::~PlayerStateStandUp()
{
}

PlayerState PlayerStateStandUp::Update(PlayerObject* owner, float deltaTime)
{
	if (!mSkelComp->IsPlaying())
	{
		return PlayerState::STATE_IDLE;
	}

	return PlayerState::STATE_STAND_UP;
}

void PlayerStateStandUp::Enter(PlayerObject* owner, float deltaTime)
{
	// 立ち上がるアニメーションを再生
	mSkelComp = owner->GetSkeltalMeshComp();
	mSkelComp->PlayAnimation(owner->GetAnim(PlayerState::STATE_STAND_UP),1.1);

	Vector3 velocity(0,0,0);
	owner->SetVelocity(velocity);
}
