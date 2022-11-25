#include "EnemyStateAttackHand.h"
#include "Game.h"
#include "SkeletalMeshComponent.h"
#include "AttackObj.h"

EnemyStateAttackHand::EnemyStateAttackHand()
	:mAttackOffsetTime(0.5f)
	,mMoveStopTime(3.2f)
	,mMoveStopTimer(0.0f)
{
}

EnemyStateAttackHand::~EnemyStateAttackHand()
{
}

EnemyState EnemyStateAttackHand::Update(EnemyObject* owner, float deltaTime)
{
	mStateTimer -= deltaTime;

	mMoveStopTimer += deltaTime;

	if (owner->GetHP() <= 0)
	{
		return EnemyState::STATE_DEATH;
	}

	// 敵からプレイヤーに向かうベクトルを算出
	Vector3 playerPos = GAMEINSTANCE.GetPlayerObjects()->GetPosition();
	Vector3 enemyToPlayerVec = playerPos - owner->GetPosition();

	float length = enemyToPlayerVec.Length();
	RotateProcess(owner, deltaTime);

	mAimVec = enemyToPlayerVec;
	mAimVec.z = 0.0f;
	mAimVec.Normalize();

	if (mMoveStopTimer >= mMoveStopTime)
	{
		Vector3 velosity = Vector3(0, 0, 0);
		owner->SetVelosity(velosity);
	}

	// アニメーションは再生中か
	if (!mSkelComp->IsPlaying())
	{
		// プレイヤーとの距離が追跡距離より短いか
		if (length < mNearLength)
		{
			return EnemyState::STATE_IDLE;
		}
		else
		{
			// 乱数を算出
			int probability = rand() % 100;

			// 30%の確立でひっかき攻撃移行
			if (0 <= probability && probability < 30)
			{
				return EnemyState::STATE_WALK;
			}

			if (70 <= probability && probability < 100)
			{
				return EnemyState::STATE_RUN;
			}
		}
	}

	Attack(owner,deltaTime);
	enemyToPlayerVec.z = 0.0f;
	enemyToPlayerVec.Normalize();
	//owner->SetDirection(enemyToPlayerVec);

	return EnemyState::STATE_ATTACK_HAND;
}

void EnemyStateAttackHand::Enter(EnemyObject* owner, float deltaTime)
{
	mStateTimer = mAttackOffsetTime;

	// アイドル状態のアニメーションを再生
	mSkelComp = owner->GetSkeletalMeshComp();
	mSkelComp->PlayAnimation(owner->GetAnim(EnemyState::STATE_ATTACK_HAND),0.3f);

	Vector3 velosity = Vector3(0,0,0);
	Vector3 dir = owner->GetDirection();
	dir.z = 0;
	dir.Normalize();

	velosity = dir * 100 * deltaTime;
	owner->SetVelosity(velosity);

	// プレイヤー座標の取得
	Vector3 playerPos = GAMEINSTANCE.GetPlayerObjects()->GetPosition();

	// 敵の座標の取得
	Vector3 ownerPos = owner->GetPosition();

	// 敵からプレイヤーに向かうベクトルを算出
	mAimVec = playerPos - ownerPos;
	mAimVec.z = 0.0f;
	mAimVec.Normalize();

	mRotateNow = true;

	//owner->ChangeLightColor(Vector3(0, 0, 1));
}

void EnemyStateAttackHand::Attack(EnemyObject* owner, float deltaTime)
{
	//攻撃可能時間になったら攻撃
	if (mStateTimer < 0.0f)
	{
		new AttackObj(Tag::EnemyAttackHand, owner->GetPosition(), Vector3(200, 200, 0), owner->GetDirection(), 300.0f, 1.3f);
		mStateTimer = FLT_MAX;		// タイマーを無限大にリセットして攻撃を繰り返さないようにする
	}
}
