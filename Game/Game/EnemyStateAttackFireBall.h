#pragma once
#include "EnemyStaeBase.h"

class EnemyStateAttackFireBall : public EnemyStateBase
{
public:
	
	EnemyStateAttackFireBall();

	~EnemyStateAttackFireBall();

	EnemyState Update(EnemyObject* owner, float deltaTime) override;

	void Enter(EnemyObject* owner, float deltaTime) override;

	void AttackProcess(EnemyObject* owner, float deltaTime);

private:

	// エネミーからプレイヤーに向かうベクトル
	Vector3 mEnemyToPlayerVec;

	// エネミーの口の座標
	Vector3 mMouthPos;
};