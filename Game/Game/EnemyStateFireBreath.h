#pragma once
#include "EnemyStaeBase.h"

class EnemyObject;

class EnemyStateFireBreath : public EnemyStateBase
{
public:
	EnemyStateFireBreath();
	~EnemyStateFireBreath();

	EnemyState Update(EnemyObject* owner, float deltaTime) override;

	void Enter(EnemyObject* owner, float deltaTime) override;

private:
	void AttackProcess(EnemyObject* owner, float deltaTime);
};