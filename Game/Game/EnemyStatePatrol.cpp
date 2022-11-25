#include "EnemyStatePatrol.h"
#include "EnemyObject.h"
#include "SkeletalMeshComponent.h"
#include "Game.h"

EnemyStatePatrol::EnemyStatePatrol()
	:mEnemyToPlayerVec(0,0,0)
{
}

EnemyStatePatrol::~EnemyStatePatrol()
{
}

EnemyState EnemyStatePatrol::Update(EnemyObject* owner, float deltaTime)
{
	Vector3 dir = owner->GetDirection();

	//回転中か?
	if (mRotateNow)
	{
		// 目標角度に近づいたら回移転終了・直進へ
		if (isNearAngle(dir, mAimVec))
		{
			dir = mAimVec;
			mRotateNow = false;
			owner->SetDirection(dir);
			owner->SetVelosity(dir * mPatrolSpeed * deltaTime);
			return EnemyState::STATE_PATROL;
		}
		// 回転
		dir = (zRotateToAimVec(dir, mAimVec, mRotateSpeed * deltaTime));
		owner->SetDirection(dir);
		owner->RotateToNewForward();
	}

	// 敵キャラの視界&範囲に入ったら追尾モードへ
	Vector3 playerPos = GAMEINSTANCE.GetPlayerObjects()->GetPosition();
	Vector3 ownerPos = owner->GetPosition();
	mEnemyToPlayerVec = playerPos - ownerPos;
	float length = mEnemyToPlayerVec.Length();


	if (InViewAngle(dir, mEnemyToPlayerVec, length))
	{
		return EnemyState::STATE_RUN;
	}

	owner->SetDirection(dir);

	// 前進
	owner->SetVelosity(dir * mPatrolSpeed * deltaTime);

	return EnemyState::STATE_PATROL;
}

void EnemyStatePatrol::Enter(EnemyObject* owner, float deltaTime)
{
	// アイドル状態のアニメーションを再生
	mSkelComp = owner->GetSkeletalMeshComp();
	mSkelComp->PlayAnimation(owner->GetAnim(EnemyState::STATE_RUN),0.2f);

	mRotateNow = false;
}
