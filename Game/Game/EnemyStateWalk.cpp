#include "EnemyStateWalk.h"
#include "Game.h"
#include "SkeletalMeshComponent.h"

EnemyStateWalk::EnemyStateWalk()
	:mWalkSpeed(200.0f)
{
}

EnemyStateWalk::~EnemyStateWalk()
{
}

EnemyState EnemyStateWalk::Update(EnemyObject* owner, float deltaTime)
{
	mStateTimer -= deltaTime;

	if (owner->GetHP() <= 0)
	{
		return EnemyState::STATE_DEATH;
	}

	// プレイヤー座標の取得
	Vector3 playerPos = GAMEINSTANCE.GetPlayerObjects()->GetPosition();
	// 敵の座標の取得
	Vector3 ownerPos = owner->GetPosition();
	// 敵からプレイヤーに向かうベクトルの長さを算出
	mEnemyToPlayerVec = playerPos - ownerPos;
	float length = mEnemyToPlayerVec.Length();
	mEnemyToPlayerVec.Normalize();

	// 敵の向きの取得
	Vector3 dir = owner->GetDirection();

	//プレイヤーとの距離が近い場合
	if (length < mFarLength && mStateTimer < 0.0)
	{
		// 乱数を算出
		int probability = rand() % 100;

		// 40%の確立でひっかき攻撃移行
		if (0 <= probability && probability < 40)
		{
			return EnemyState::STATE_PRELIMINARY_ACTION_HAND;
		}

		// 40%の確立で噛みつき攻撃に移行
		if (40 <= probability && probability < 80)
		{
			return EnemyState::STATE_PRELIMINARY_ACTION_BITE;
		}

		// 20%の確立でラン状態に移行
		if (80 <= probability && probability < 100)
		{
			return EnemyState::STATE_RUN;
		}
	}

	// プレイヤーとの距離が遠い場合
	if (length > mNearLength && mStateTimer < 0.0)
	{
		// 乱数を算出
		int probability = rand() % 100;

		// 30%の確立でラン状態に移行
		if (0 <= probability && probability < 30)
		{
			return EnemyState::STATE_RUN;
		}

		// 35%の確立でブレス攻撃に移行
		if (30 <= probability && probability < 65)
		{
			return EnemyState::STATE_FIRE_BREATH;
		}

		// 35%の確立で火球攻撃に移行
		if (65 <= probability && probability < 100)
		{
			return EnemyState::STATE_FIRE_BALL;
		}
	}

	// 敵からプレイヤーに向かうベクトルを敵の向きにセット
	mEnemyToPlayerVec.z = 0.0f;
	mEnemyToPlayerVec.Normalize();
	owner->SetDirection(mEnemyToPlayerVec);

	//移動処理
	MoveCalc(owner, deltaTime);

	return EnemyState::STATE_WALK;
}

void EnemyStateWalk::Enter(EnemyObject* owner, float deltaTime)
{
	// アニメーションの再生
	mSkelComp = owner->GetSkeletalMeshComp();
	mSkelComp->PlayAnimation(owner->GetAnim(EnemyState::STATE_WALK),0.4f);

	mStateTimer = 2.0f;
}

void EnemyStateWalk::MoveCalc(EnemyObject* owner, float deltaTime)
{
		// プレイヤーに向かうベクトル
		Vector3 dir = mEnemyToPlayerVec;
		dir.z = 0.0f;
		dir.Normalize();

		Vector3 velocity = dir * mWalkSpeed * deltaTime;
		owner->SetVelosity(velocity);

		owner->SetDirection(dir);
		owner->SetComputeWorldTransform();
}
