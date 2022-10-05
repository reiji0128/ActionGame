#include "EnemyStateAttackBite.h"
#include "EnemyObject.h"
#include "Game.h"
#include "SkeletalMeshComponent.h"
#include "AttackObj.h"

EnemyStateAttackBite::EnemyStateAttackBite()
{
}

EnemyStateAttackBite::~EnemyStateAttackBite()
{
}

EnemyState EnemyStateAttackBite::Update(EnemyObject* owner, float deltaTime)
{
	// 敵からプレイヤーに向かうベクトルを算出
	Vector3 playerPos = GAMEINSTANCE.GetPlayerObjects()->GetPosition();
	Vector3 enemyToPlayerVec = playerPos - owner->GetPosition();

	float length = enemyToPlayerVec.Length();


	// アニメーションは再生中か
	if (!mSkelComp->IsPlaying())
	{
		// プレイヤーとの距離が追跡する距離よりも短いか
		if (length < retrackingRange)
		{
			return EnemyState::STATE_IDLE;
		}
		else
		{
			return EnemyState::STATE_RUN;
		}
	}

	return EnemyState::STATE_ATTACK_BITE;
}

void EnemyStateAttackBite::Enter(EnemyObject* owner, float deltaTime)
{
	mSkelComp = owner->GetSkeletalMeshComp();
	mSkelComp->PlayAnimation(owner->GetAnim(EnemyState::STATE_ATTACK_BITE));

	// 攻撃処理
	Attack(owner, deltaTime);
}

void EnemyStateAttackBite::Attack(EnemyObject* owner, float deltaTime)
{
	if (mStateTimer < 0.0)
	{
		new AttackObj(Tag::EnemyAttackHand, owner->GetPosition(), owner->GetDirection(), 150.0f, 0.3f);

		// タイマーを無限大にリセットして攻撃を繰り返さないようにする
		mStateTimer = FLT_MAX;
	}

}
