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

	// 攻撃アニメ開始してから攻撃判定が発生するまでの調整時間
	const float mAttackOffsetTime;

	// 止まるまでの時間
	const float mMoveStopTime;

	float mMoveStopTimer;
};