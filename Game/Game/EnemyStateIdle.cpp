#include "EnemyStateIdle.h"
#include "EnemyObject.h"
#include "SkeletalMeshComponent.h"
#include "Game.h"

EnemyStateIdle::EnemyStateIdle()
{
}

EnemyStateIdle::~EnemyStateIdle()
{
}


EnemyState EnemyStateIdle::Update(EnemyObject* owner, float deltaTime)
{
	mStateTimer -= deltaTime;

	// プレイヤーの座標取得
	Vector3 playerPos = GAMEINSTANCE.GetPlayerObjects()->GetPosition();
	// 敵の座標取得
	Vector3 mPosition = owner->GetPosition();
	// 敵からプレイヤーに向かうベクトルを算出
	Vector3 enemyToPlayerVec = playerPos - mPosition;
	float length = enemyToPlayerVec.Length();

	// 敵の向きの取得
	Vector3 dir = owner->GetDirection();

	//return EnemyState::STATE_IDLE;

	//近距離攻撃の距離まで近づいていて,ステートのタイマーが0になったら近距離攻撃状態に移行
	if (InViewAngle(dir,enemyToPlayerVec,length) || mStateTimer < 0.0f)
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

		// 10%の確立で火ブレス攻撃に移行
		if (80 <= probability && probability < 90)
		{
			return EnemyState::STATE_FIRE_BREATH;
		}

		// 10%の確立で火球攻撃に移行
		if (90 <= probability && probability < 100)
		{
			return EnemyState::STATE_FIRE_BALL;
		}
	}

	// プレイヤーとの距離が遠かったら
	if (length <= mFarLength && mStateTimer < 0.0f)
	{
		// 乱数を算出
		int probability = rand() % 100;
		
		// 50%の確立でラン状態移行
		if (0 <= probability && probability < 30)
		{
			return EnemyState::STATE_WALK;
		}

		// 50%の確立でラン状態移行
		if (30 <= probability && probability < 40)
		{
			return EnemyState::STATE_RUN;
		}

		// 25%の確立でブレス攻撃に移行
		if (40 <= probability && probability < 70)
		{
			return EnemyState::STATE_FIRE_BREATH;
		}

		// 25%の確立で火球攻撃に移行
		if (70 <= probability && probability < 100)
		{
			return EnemyState::STATE_FIRE_BALL;
		}
	}
	
	// 視野に入っていなかったら
	if (!InViewAngle(dir, enemyToPlayerVec, length))
	{
		enemyToPlayerVec.z = 0.0f;
		enemyToPlayerVec.Normalize();
		owner->SetDirection(enemyToPlayerVec);
	}

	if (mStateTimer < 0.0f)
	{
		return EnemyState::STATE_RUN;
	}

	if (owner->GetIsDamage())
	{
		//return EnemyState::STATE_DAMAGE;
	}

	enemyToPlayerVec.z = 0.0f;
	enemyToPlayerVec.Normalize();
	owner->SetDirection(enemyToPlayerVec);

	return EnemyState::STATE_IDLE;
}

void EnemyStateIdle::Enter(EnemyObject* owner, float deltaTime)
{
	// アイドル状態のアニメーションを再生
	mSkelComp = owner->GetSkeletalMeshComp();
	mSkelComp->PlayAnimation(owner->GetAnim(EnemyState::STATE_IDLE),0.3f);
	
	Vector3 velosity = Vector3(0, 0, 0);
	owner->SetVelosity(velosity);
	mStateTimer = 1.5f;
}
