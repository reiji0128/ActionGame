#include "EnemyStateAttackHand.h"
#include "Game.h"
#include "SkeletalMeshComponent.h"
#include "AttackObj.h"

EnemyStateAttackHand::EnemyStateAttackHand()
{
}

EnemyStateAttackHand::~EnemyStateAttackHand()
{
}

EnemyState EnemyStateAttackHand::Update(EnemyObject* owner, float deltaTime)
{
	mStateTimer -= deltaTime;

	// プレイヤーから離れたら追跡モードへ
	Vector3 playerPos = GAMEINSTANCE.GetPlayerObjects()->GetPosition();
	Vector3 enemyToPlayerVec = playerPos - owner->GetPosition();

	float length = enemyToPlayerVec.Length();

	// プレイヤーが離れたら追跡モードへ移行
	if (length > retrackingRange)
	{
		return EnemyState::STATE_RUN;
	}

	// 再生アニメが完了したら追跡モードへ移行
	if (!mSkelComp->IsPlaying())
	{
		return EnemyState::STATE_RUN;
	}

	////プレイヤーの攻撃を受けたか
	//if (owner->GetIsHitTrig())
	//{
	//	return EnemyState::STATE_DEATH;
	//}
	return EnemyState::STATE_ATTACK_HAND;
}

void EnemyStateAttackHand::Enter(EnemyObject* owner, float deltaTime)
{
	// アイドル状態のアニメーションを再生
	mSkelComp = owner->GetSkeletalMeshComp();
	mSkelComp->PlayAnimation(owner->GetAnim(EnemyState::STATE_ATTACK_HAND),0.8f);

	Vector3 velosity = Vector3(0,0,0);
	owner->SetVelosity(velosity);
}

void EnemyStateAttackHand::Attack(EnemyObject* owner, float deltaTime)
{
	//攻撃可能時間になったら攻撃
	if (mStateTimer < 0.0f)
	{
		new AttackObj(Tag::EnemyAttackHand, owner->GetPosition(), owner->GetDirection(), 150.0f, 0.3f);
		mStateTimer = FLT_MAX;		// タイマーを無限大にリセットして攻撃を繰り返さないようにする
	}
}
