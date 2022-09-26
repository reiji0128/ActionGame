#pragma once

#pragma once
#include "EnemyStaeBase.h"

class EnemyObject;

class EnemyStateRun : public EnemyStateBase
{
public:
	EnemyStateRun();

	~EnemyStateRun() {};

	EnemyState Update(EnemyObject* owner, float deltaTime) override;

	void Enter(EnemyObject* owner, float deltaTime) override;

private:
	void MoveCalc(EnemyObject* owner, float deltaTime);

	Vector3 enemyToPlayerVec;

	const float nearLength = 300.0f;

	const float trackingRange = 500.0f;

	const float runSpeed = 150.0f;   //í«ê’ë¨ìx

};