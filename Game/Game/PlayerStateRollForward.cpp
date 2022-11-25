#include "PlayerStateRollForward.h"
#include "Input.h"

PlayerStateRollForward::PlayerStateRollForward()
	:mSpeed(300)
{
}

PlayerStateRollForward::~PlayerStateRollForward()
{
}

PlayerState PlayerStateRollForward::Update(PlayerObject* owner, float deltaTime)
{
	// 回転の計算
	RollCalc(owner, deltaTime);

	// アニメーションの再生が終わっていたら
	if (!mSkelComp->IsPlaying())
	{
		owner->SetVelocity(Vector3::Zero);

		return PlayerState::STATE_IDLE;
	}

	// ダメージを受けていたらダメージ受ける状態に移行
	if (owner->GetIsDamage())
	{
		return PlayerState::STATE_FLYING_BACK;
	}

	return PlayerState::STATE_ROLL_FORWARD;
}

void PlayerStateRollForward::Enter(PlayerObject* owner, float deltaTime)
{
	// アニメーションを再生
	mSkelComp = owner->GetSkeltalMeshComp();
	mSkelComp->PlayAnimation(owner->GetAnim(PlayerState::STATE_ROLL_FORWARD), 0.8f);

	// 座標の取得
	mOwnerPos = owner->GetPosition();

	// 前方ベクトルの取得
	mCharaForwardVec = owner->GetDirection();
}

void PlayerStateRollForward::RollCalc(PlayerObject* owner, float deltaTime)
{
	Vector3 upVec(0.0f,0.0f,1.0f);
	Vector3 rightVec;

	// 前方ベクトルと上軸(Z軸)から右ベクトルを算出
	rightVec = Vector3::Cross(mCharaForwardVec, upVec);

	if (mCharaForwardVec.Length() > 0.5)
	{
		mCharaForwardVec.Normalize();;
	}

	// 前方向に回転
	mCharaForwardVec.z = 0;
	Vector3 velocity;
	velocity += mSpeed * mCharaForwardVec;

	// 最高速度を超えていたら調整
	Vector3 horizonSpeed = velocity;
	if (horizonSpeed.Length() > mSpeed * deltaTime)
	{
		horizonSpeed = mCharaForwardVec * mSpeed * deltaTime;
		velocity.x = horizonSpeed.x;
		velocity.y = horizonSpeed.y;
		velocity.z = 0;
	}

	// スピード・変換行列の再計算の必要をセット
	owner->SetVelocity(velocity);
	owner->SetComputeWorldTransform();
}
