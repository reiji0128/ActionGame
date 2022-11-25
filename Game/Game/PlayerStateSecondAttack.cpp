#include "PlayerStateSecondAttack.h"
#include "Input.h"
#include "AttackObj.h"

PlayerStateSecondAttack::PlayerStateSecondAttack()
{
}

PlayerStateSecondAttack::~PlayerStateSecondAttack()
{
}

PlayerState PlayerStateSecondAttack::Update(PlayerObject* owner, float deltaTime)
{
	mStateTimer -= deltaTime;
	// 攻撃処理
	Attack(owner,deltaTime);

	// 入力処理
	InputProcess();

	// アニメーションの再生が終わっているか
	if (!mSkelComp->IsPlaying())
	{
		// 移動入力がなかったらアイドル状態に移行
		// 移動入力がされたらラン状態に移行
		if (IsIdle)
		{
			return PlayerState::STATE_IDLE;
		}
		else
		{
			return PlayerState::STATE_RUN;
		}
	}

	// ダメージを受けていたらダメージ受ける状態に移行
	if (owner->GetIsDamage())
	{
		return PlayerState::STATE_FLYING_BACK;
	}

	return PlayerState::STATE_SECOND_ATTACK;
}

void PlayerStateSecondAttack::Enter(PlayerObject* owner, float deltaTime)
{
	mSkelComp = owner->GetSkeltalMeshComp();
	mSkelComp->PlayAnimation(owner->GetAnim(PlayerState::STATE_SECOND_ATTACK),0.7f);
	mStateTimer = 1.0f;

	owner->SetVelocity(Vector3(0, 0, 0));
}

void PlayerStateSecondAttack::Attack(PlayerObject* owner, float deltaTime)
{
	//攻撃可能時間になったら攻撃
	if (mStateTimer < 0.0f)
	{
		new AttackObj(Tag::PlayerSecondAttack, owner->GetPosition(), owner->GetDirection(), 300.0f, 0.01f);

		// タイマーを無限大にリセットして攻撃を繰り返さないようにする
		mStateTimer = FLT_MAX;
	}
}

void PlayerStateSecondAttack::InputProcess()
{
	// コントローラの左スティックが入力されたか
	Vector2 stickL = INPUT_INSTANCE.GetLStick();
	IsControllerInputOff = !INPUT_INSTANCE.IsLStickMove();

	// 移動入力がされていないか
	IsIdle = INPUT_INSTANCE.IsKeyOff(KEY_W) &
		     INPUT_INSTANCE.IsKeyOff(KEY_S) &
		     INPUT_INSTANCE.IsKeyOff(KEY_D) &
		     INPUT_INSTANCE.IsKeyOff(KEY_A) &
		     IsControllerInputOff;
}
