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

	void RotateProcess(EnemyObject* owner, float deltaTime);

	void Attack(EnemyObject* owner, float deltaTime);

	Vector3 mEnemyToPlayerVec;

	Vector3 mPlayerPos;

	const float nearLength = 300.0f;

	const float trackingRange = 500.0f;

	const float runSpeed;   //í«ê’ë¨ìx

};