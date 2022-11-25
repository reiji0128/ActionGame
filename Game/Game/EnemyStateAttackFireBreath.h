#pragma once
#include "EnemyStaeBase.h"

// ‘O•ûéŒ¾
class EnemyObject;
class FireBreathObject;

class EnemyStateAttackFireBreath : public EnemyStateBase
{
public:
	EnemyStateAttackFireBreath();
	~EnemyStateAttackFireBreath();

	EnemyState Update(EnemyObject* owner, float deltaTime) override;

	void Enter(EnemyObject* owner, float deltaTime) override;

private:
	void AttackProcess(EnemyObject* owner, float deltaTime);

	Vector3 mEnemyToPlayerVec;

	Vector3 mMouthPos;

	FireBreathObject* mFireBreath;

};