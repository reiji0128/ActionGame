#pragma once
#include "EnemyStaeBase.h"

class EnemyStatePreliminaryActionHand : public EnemyStateBase
{
public:

	EnemyStatePreliminaryActionHand();

	~EnemyStatePreliminaryActionHand();

	EnemyState Update(EnemyObject* owner, float deltaTime) override;

	void Enter(EnemyObject* owner, float deltaTime) override;
};
