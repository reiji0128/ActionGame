#pragma once
#include "EnemyStaeBase.h"

class EnemyObject;

class EnemyStateAttackHand : public EnemyStateBase
{
public:
	EnemyStateAttackHand();

	~EnemyStateAttackHand();

	EnemyState Update(EnemyObject* owner, float deltaTime) override;

	void Enter(EnemyObject* owner, float deltaTime) override;

private:
	void Attack(EnemyObject* owner, float deltaTime);

	// 攻撃中から追跡に変わる距離
	const float retrackingRange = 300.0f;
	// 攻撃アニメ開始してから攻撃判定が発生するまでの調整時間
	const float attackOffsetTime = 0.5f;
};