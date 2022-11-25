#include "EnemyStaeBase.h"
#include "Game.h"

EnemyStateBase::EnemyStateBase()
	:mStateTimer(0.0f)
	,mSkelComp(nullptr)
{
}

void EnemyStateBase::RotateProcess(EnemyObject* owner, float deltaTime)
{
	// 現在の向き
	Vector3 nowDirection = owner->GetDirection();

	// 現在の座標
	Vector3 ownerPos = owner->GetPosition();

	// プレイヤーの座標
	Vector3 playerPos = GAMEINSTANCE.GetPlayerObjects()->GetPosition();

	// 回転中か
	if (mRotateNow)
	{
		// 二つのベクトルがほぼ同じか
		if (isNearAngle(nowDirection, mAimVec))
		{
			mRotateNow = false;

			// 向きのセット
			owner->SetDirection(nowDirection);
		}

		// 回転処理
		nowDirection = zRotateToAimVec(nowDirection, mAimVec, mRotateSpeed * deltaTime);
		owner->SetDirection(nowDirection);
		owner->RotateToNewForward();
	}
}

bool EnemyStateBase::InViewAngle(const Vector3& from, const Vector3& to, float length)
{
	// 二つのベクトルから角度(弧度法)を計算
	float angle = Vector3::Angle(from, to);

	// 視野の範囲内に入っていたら
	if (angle < viewAngle && length < mNearLength)
	{
		return true;
	}

	return false;
}
