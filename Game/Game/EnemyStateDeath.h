#pragma once
#include "EnemyStaeBase.h"

class EnemyObject;

class EnemyStateDeath : public EnemyStateBase
{
public:
	EnemyStateDeath();

	~EnemyStateDeath();

	EnemyState Update(EnemyObject* owner, float deltaTime) override;

	void Enter(EnemyObject* owner, float deltaTime) override;

private:
};
