#include "PlayerStateSecondAttack.h"
#include "Input.h"

PlayerStateSecondAttack::PlayerStateSecondAttack()
{
}

PlayerStateSecondAttack::~PlayerStateSecondAttack()
{
}

PlayerState PlayerStateSecondAttack::Update(PlayerObject* owner, float deltaTime)
{
	// コントローラの左スティックが入力されたか
	Vector2 stickL = INPUT_INSTANCE.GetLStick();
	bool isContollerInputOff = !INPUT_INSTANCE.IsLStickMove();

	bool IsIdle = INPUT_INSTANCE.IsKeyOff(KEY_W) &
		          INPUT_INSTANCE.IsKeyOff(KEY_S) &
		          INPUT_INSTANCE.IsKeyOff(KEY_D) &
		          INPUT_INSTANCE.IsKeyOff(KEY_A) &
		          isContollerInputOff;

	// 移動入力がなかったら
	if (IsIdle)
	{
		return PlayerState::STATE_IDLE;
	}
	else
	{
		return PlayerState::STATE_RUN;
	}
}

void PlayerStateSecondAttack::Enter(PlayerObject* owner, float deltaTime)
{
	mSkelComp = owner->GetSkeltalMeshComp();
	mSkelComp->PlayAnimation(owner->GetAnim(PlayerState::STATE_SECOND_ATTACK));
}
