#pragma once
#include "EnemyStaeBase.h"

class EnemyObject;

class EnemyStatePatrol : public EnemyStateBase
{
public:
	EnemyStatePatrol();

	~EnemyStatePatrol();

	EnemyState Update(EnemyObject* owner, float deltaTime) override;

	void Enter(EnemyObject* owner, float deltaTime) override;

private:
	const float mPatrolSpeed = 100.0f;   //パトロール時のスピード
	const float mRotateSpeed = 45.0f;    // 回転スピード
	const float mTrackingRange = 500.0f; // 追跡距離

	Vector3 mEnemyToPlayerVec;			// エネミーからプレイヤーに向かうベクトル
	Vector3 mAimVec;                    // 目標回転方向
	bool mRotateNow;                    // 回転中か？
};