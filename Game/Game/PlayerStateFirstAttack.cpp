#include "PlayerStateFirstAttack.h"
#include "Input.h"
#include "AttackObj.h"

PlayerStateFirstAttack::PlayerStateFirstAttack()
	:NextComboTimer(0.0f)
{
}

PlayerStateFirstAttack::~PlayerStateFirstAttack()
{
}

PlayerState PlayerStateFirstAttack::Update(PlayerObject* owner, float deltaTime)
{
	mStateTimer -= deltaTime;

	NextComboTimer += deltaTime;

	// 攻撃処理
	Attack(owner, deltaTime);

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

	// コンボ入力があったら次のコンボに移行
	if (IsNextCombo)
	{
		IsNextCombo = false;
		return PlayerState::STATE_SECOND_ATTACK;
	}

	// ダメージを受けていたらダメージ受ける状態に移行
	if (owner->GetIsDamage())
	{
		return PlayerState::STATE_FLYING_BACK;
	}

	return PlayerState::STATE_FIRST_ATTACK;
}

void PlayerStateFirstAttack::Enter(PlayerObject* owner, float deltaTime)
{
	// アイドル状態のアニメーションを再生
	mSkelComp = owner->GetSkeltalMeshComp();
	mSkelComp->PlayAnimation(owner->GetAnim(PlayerState::STATE_FIRST_ATTACK),0.5f);
	
	mStateTimer = 1.0f;
	NextComboTimer = 0.0f;

	owner->SetVelocity(Vector3(0, 0, 0));
}

void PlayerStateFirstAttack::Attack(PlayerObject* owner, float deltaTime)
{
	//攻撃可能時間になったら攻撃
	if (mStateTimer < 0.0f)
	{
		new AttackObj(Tag::PlayerFirstAttack, owner->GetPosition(), owner->GetDirection(), 300.0f, 0.01f);

		// タイマーを無限大にリセットして攻撃を繰り返さないようにする
		mStateTimer = FLT_MAX;
	}
}

void PlayerStateFirstAttack::InputProcess()
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

	// 次コンボに移行できるフレーム以降に攻撃入力がされたか
	if (INPUT_INSTANCE.IsKeyPushdown(BUTTON_B) && NextComboTimer >= NextComboLimitTimer)
	{
		IsNextCombo = true;
	}

}
