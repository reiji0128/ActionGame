#include "EnemyStateRun.h"
#include "SkeletalMeshComponent.h"
#include "Game.h"

EnemyStateRun::EnemyStateRun()
{
}

EnemyState EnemyStateRun::Update(EnemyObject* owner, float deltaTime)
{
	// プレイヤー位置情報
	Vector3 playerPos = GAMEINSTANCE.GetPlayerObjects()->GetPosition();
	Vector3 pos = owner->GetPosition();
	enemyToPlayerVec = playerPos - pos;

	float length = enemyToPlayerVec.Length();
	enemyToPlayerVec.Normalize();

	//追跡しない距離まで近づいたときに攻撃に移行
	if (length < nearLength)
	{
		return EnemyState::STATE_ATTACK_HAND;
	}

	// 追跡可能距離を超えたらステートをパトロールに移行
	if (length > trackingRange)
	{
		return EnemyState::STATE_PATROL;
	}

	//移動処理
	MoveCalc(owner, deltaTime);
	return EnemyState::STATE_RUN;
}

void EnemyStateRun::Enter(EnemyObject* owner, float deltaTime)
{
	mSkelComp = owner->GetSkeletalMeshComp();
	mSkelComp->PlayAnimation(owner->GetAnim(EnemyState::STATE_RUN),0.3f);
}

void EnemyStateRun::MoveCalc(EnemyObject* owner, float deltaTime)
{
	//水平方向の向きを取得
	Vector3 dir = enemyToPlayerVec;
	dir.z = 0.0f;
	dir.Normalize();

	Vector3 velocity = dir * runSpeed * deltaTime;
	owner->SetVelosity(velocity);

	owner->SetDirection(dir);
	owner->SetComputeWorldTransform();
}
