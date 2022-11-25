#pragma once
#include "EnemyStaeBase.h"

class EnemyStateDamage : public EnemyStateBase
{
public:
	EnemyStateDamage();

	~EnemyStateDamage();

	EnemyState Update(EnemyObject* owner, float deltaTime) override;

	void Enter(EnemyObject* owner, float deltaTime) override;
};