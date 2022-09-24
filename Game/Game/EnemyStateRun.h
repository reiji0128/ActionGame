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

};