#pragma once
#include "EnemyStaeBase.h"

class EnemyObject;

class EnemyStateAttackHand : public EnemyStateBase
{
public:
	EnemyStateAttackHand();

	~EnemyStateAttackHand();

	EnemyState Update(EnemyObject* owner, float deltaTime) override;

	void Enter(EnemyObject* owner, float deltaTime) override;

private:

	void Attack(EnemyObject* owner, float deltaTime);

	// UŒ‚ƒAƒjƒŠJn‚µ‚Ä‚©‚çUŒ‚”»’è‚ª”­¶‚·‚é‚Ü‚Å‚Ì’²®ŠÔ
	const float mAttackOffsetTime;

	// ~‚Ü‚é‚Ü‚Å‚ÌŠÔ
	const float mMoveStopTime;

	float mMoveStopTimer;
};