#pragma once
#include "EnemyStaeBase.h"

class EnemyObject;

class EnemyStateIdle : public EnemyStateBase
{
public:
	EnemyStateIdle();

	~EnemyStateIdle();

	EnemyState Update(EnemyObject* owner, float deltaTime) override;

	void Enter(EnemyObject* owner, float deltaTime) override;

private:
	// プレイヤーとの距離
	const float mNearLength = 250.0f;

	// 待機時間
	const float mWaitTime = 2.0f;

};