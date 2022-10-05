#include "PlayerStateFirstAttack.h"
#include "Input.h"
#include "AttackObj.h"

PlayerStateFirstAttack::PlayerStateFirstAttack()
{
}

PlayerStateFirstAttack::~PlayerStateFirstAttack()
{
}

PlayerState PlayerStateFirstAttack::Update(PlayerObject* owner, float deltaTime)
{
	mStateTimer -= deltaTime;

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

void PlayerStateFirstAttack::Enter(PlayerObject* owner, float deltaTime)
{
	// アイドル状態のアニメーションを再生
	mSkelComp = owner->GetSkeltalMeshComp();
	mSkelComp->PlayAnimation(owner->GetAnim(PlayerState::STATE_FIRST_ATTACK));
	
	Attack(owner, deltaTime);
	
	IsFirstAttack = true;
}

void PlayerStateFirstAttack::Attack(PlayerObject* owner, float deltaTime)
{
	//攻撃可能時間になったら攻撃
	if (mStateTimer < 0.0f)
	{
		new AttackObj(Tag::EnemyAttackHand, owner->GetPosition(), owner->GetDirection(), 150.0f, 0.3f);

		// タイマーを無限大にリセットして攻撃を繰り返さないようにする
		mStateTimer = FLT_MAX;
	}
}
