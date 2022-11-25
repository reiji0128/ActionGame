#include "PlayerStateFlyingBack.h"
#include "SkeletalMeshComponent.h"
#include "Game.h"

PlayerStateFlyingBack::PlayerStateFlyingBack()
	:mBackSpeed(0.0f)
	,mInitBackSpeed(400.0f)
	,mDecelerationSpeed(0.0f)
	,mInitDecelerationSpeed(5.8f)
{
}

PlayerStateFlyingBack::~PlayerStateFlyingBack()
{
}

PlayerState PlayerStateFlyingBack::Update(PlayerObject* owner, float deltaTime)
{

	Vector3 velocity = mBackSpeed * mPlayerToEnemyVec;
	mBackSpeed -= mDecelerationSpeed;
	mDecelerationSpeed += 0.05f;

	if (mBackSpeed <= 0.0f)
	{
		mBackSpeed = 0.0f;
	}

	mOwnerPos -= velocity * deltaTime;
	owner->SetPosition(mOwnerPos);

	if (!mSkelComp->IsPlaying())
	{
		owner->SetIsDamage(false);
		return PlayerState::STATE_STAND_UP;
	}

	// HPが0以下になったら死亡状態に移行
	if (owner->GetHP() <= 0)
	{
		return PlayerState::STATE_DEATH;
	}

	return PlayerState::STATE_FLYING_BACK;
}

void PlayerStateFlyingBack::Enter(PlayerObject* owner, float deltaTime)
{
	mSkelComp = owner->GetSkeltalMeshComp();
	mSkelComp->PlayAnimation(owner->GetAnim(PlayerState::STATE_FLYING_BACK));

	// スピードの初期化
	mBackSpeed = mInitBackSpeed;
	mDecelerationSpeed = mInitDecelerationSpeed;

	Vector3 enemyPos = GAMEINSTANCE.GetFirstGameObject(Tag::Enemy)->GetPosition();
	mOwnerPos = owner->GetPosition();
	mPlayerToEnemyVec = enemyPos - mOwnerPos;
	mPlayerToEnemyVec.z = 0;
	mPlayerToEnemyVec.Normalize();

	// ダメージ処理
	int damage = owner->GetReceiveDamage();
	owner->Damage(damage);
}
