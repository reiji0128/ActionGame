#include "EnemyStateDetection.h"
#include "SkeletalMeshComponent.h"
#include "Game.h"

EnemyStateDetection::EnemyStateDetection()
	:mRunSpeed(200.0f)
{
}

EnemyStateDetection::~EnemyStateDetection()
{
}

EnemyState EnemyStateDetection::Update(EnemyObject* owner, float deltaTime)
{
	// プレイヤーの座標取得
	Vector3 playerPos = GAMEINSTANCE.GetPlayerObjects()->GetPosition();
	// 敵の座標取得
	Vector3 position = owner->GetPosition();
	// 敵からプレイヤーに向かうベクトルを算出
	Vector3 enemyToPlayerVec = playerPos - position;
	float length = enemyToPlayerVec.Length();

	// 敵の向きの取得
	Vector3 dir = owner->GetDirection();

	if (length <= mNearLength)
	{
		return EnemyState::STATE_PRELIMINARY_ACTION_HAND;
	}

	enemyToPlayerVec.z = 0.0f;
	enemyToPlayerVec.Normalize();
	owner->SetDirection(enemyToPlayerVec);

	MoveCalc(owner, deltaTime, enemyToPlayerVec);

	return EnemyState::STATE_DETECTION;
}

void EnemyStateDetection::Enter(EnemyObject* owner, float deltaTime)
{
	// アイドル状態のアニメーションを再生
	mSkelComp = owner->GetSkeletalMeshComp();
	mSkelComp->PlayAnimation(owner->GetAnim(EnemyState::STATE_DETECTION),0.4f);

	Vector3 velocity = Vector3::Zero;
	owner->SetVelosity(velocity);

	//owner->ChangeLightColor(Vector3(0, 0, 0));
}

void EnemyStateDetection::MoveCalc(EnemyObject* owner, float deltaTime,const Vector3& targetDir)
{
	//水平方向の向きを取得
	Vector3 dir = targetDir;
	dir.z = 0.0f;
	dir.Normalize();

	Vector3 velocity = dir * mRunSpeed * deltaTime;
	owner->SetVelosity(velocity);

	owner->SetDirection(dir);
	owner->SetComputeWorldTransform();
}
