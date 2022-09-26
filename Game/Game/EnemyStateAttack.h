#pragma once
#include "EnemyStaeBase.h"

class EnemyObject;

class EnemyStateAttack : public EnemyStateBase
{
public:
	EnemyStateAttack();

	~EnemyStateAttack();

	EnemyState Update(EnemyObject* owner, float deltaTime) override;

	void Enter(EnemyObject* owner, float deltaTime) override;
};