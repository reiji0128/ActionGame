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

	// プレイヤー位置情報
	Vector3 playerPos = GAMEINSTANCE.GetPlayerObjects()->GetPosition();
	Vector3 mPosition = owner->GetPosition();
	Vector3 enemyToPlayerVec = playerPos - mPosition;

	float length = enemyToPlayerVec.Length();

	//追跡しない距離まで近づいているか
	if (length < mNearLength)
	{
		int probability = rand() % 100;

		if (0 <= probability && probability >50)
		{
			return EnemyState::STATE_ATTACK_HAND;
		}
		if (50 <= probability && probability > 100)
		{
			return EnemyState::STATE_ATTACK_BITE;
		}
	}

	if (mStateTimer < 0.0f)
	{
		return EnemyState::STATE_PATROL;
	}

	return EnemyState::STATE_IDLE;
}

void EnemyStateIdle::Enter(EnemyObject* owner, float deltaTime)
{
	// アイドル状態のアニメーションを再生
	mSkelComp = owner->GetSkeletalMeshComp();
	mSkelComp->PlayAnimation(owner->GetAnim(EnemyState::STATE_IDLE));
}
