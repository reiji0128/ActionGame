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

	// UŒ‚’†‚©‚ç’ÇÕ‚É•Ï‚í‚é‹——£
	const float retrackingRange = 300.0f;
	// UŒ‚ƒAƒjƒŠJn‚µ‚Ä‚©‚çUŒ‚”»’è‚ª”­¶‚·‚é‚Ü‚Å‚Ì’²®ŠÔ
	const float attackOffsetTime = 0.5f;
};