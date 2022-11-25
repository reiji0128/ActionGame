#pragma once
#include "EnemyStaeBase.h"

class EnemyStateWalk : public EnemyStateBase
{
public:
	EnemyStateWalk();

	~EnemyStateWalk();

	EnemyState Update(EnemyObject* owner, float deltaTime) override;

	void Enter(EnemyObject* owner, float deltaTime) override;

private:

	void MoveCalc(EnemyObject* owner, float deltaTime);

	Vector3 mEnemyToPlayerVec;

	const float mWalkSpeed;
};