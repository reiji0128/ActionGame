#include "PlayerStateDamage.h"

PlayerStateDamage::PlayerStateDamage()
{
}

PlayerStateDamage::~PlayerStateDamage()
{
}

PlayerState PlayerStateDamage::Update(PlayerObject* owner, float deltaTime)
{
	// アニメーションの再生が終わったらアイドル状態に移行
	if (!mSkelComp->IsPlaying())
	{
		owner->SetIsDamage(false);
		return PlayerState::STATE_IDLE;
	}

	// HPが0以下になったら死亡状態に移行
	if (owner->GetHP() <= 0)
	{
		return PlayerState::STATE_DEATH;
	}

	return PlayerState::STATE_DAMAGE;
}

void PlayerStateDamage::Enter(PlayerObject* owner, float deltaTime)
{
	// アイドル状態のアニメーションを再生
	mSkelComp = owner->GetSkeltalMeshComp();
	mSkelComp->PlayAnimation(owner->GetAnim(PlayerState::STATE_DAMAGE));

	// ダメージ処理
	int damage = owner->GetReceiveDamage();
	owner->Damage(damage);
}
