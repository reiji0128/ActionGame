#include "EnemyStateDamage.h"
#include "Game.h"
#include "SkeletalMeshComponent.h"

EnemyStateDamage::EnemyStateDamage()
{
}

EnemyStateDamage::~EnemyStateDamage()
{
}

EnemyState EnemyStateDamage::Update(EnemyObject* owner, float deltaTime)
{
	
	//// 乱数を算出
	//int probability = rand() % 100;

	//// 40%の確立でひっかき攻撃移行
	//if (0 <= probability && probability < 40)
	//{
	//	return EnemyState::STATE_PRELIMINARY_ACTION_HAND;
	//}

	//// 40%の確立で噛みつき攻撃に移行
	//if (40 <= probability && probability < 80)
	//{
	//	return EnemyState::STATE_PRELIMINARY_ACTION_BITE;
	//}

	//// 10%の確立で火ブレス攻撃に移行
	//if (80 <= probability && probability < 90)
	//{
	//	return EnemyState::STATE_WALK;
	//}

	//// 10%の確立で火球攻撃に移行
	//if (90 <= probability && probability < 100)
	//{
	//	return EnemyState::STATE_RUN;
	//}
	return EnemyState::STATE_IDLE;
	if (owner->GetHP() <= 0)
	{
		return EnemyState::STATE_DEATH;
	}
}

void EnemyStateDamage::Enter(EnemyObject* owner, float deltaTime)
{
	// アイドル状態のアニメーションを再生
	mSkelComp = owner->GetSkeletalMeshComp();
	//mSkelComp->PlayAnimation(owner->GetAnim(EnemyState::STATE_DAMAGE),0.6);

	mStateTimer = 2.0f;

	// ダメージ処理
	int damage = owner->GetReceiveDamage();
	owner->Damage(damage);
}
