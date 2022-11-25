#pragma once
#include "EnemyStaeBase.h"

class EnemyStatePreliminaryActionBite : public EnemyStateBase
{
public:
	EnemyStatePreliminaryActionBite();

	~EnemyStatePreliminaryActionBite();

	EnemyState Update(EnemyObject* owner, float deltaTime) override;

	void Enter(EnemyObject* owner, float deltaTime) override;
};