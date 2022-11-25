#pragma once
#include "EnemyStaeBase.h"

class EnemyObject;

class EnemyStateAttackBite : public EnemyStateBase
{
public:
	EnemyStateAttackBite();

	~EnemyStateAttackBite();

	EnemyState Update(EnemyObject* owner, float deltaTime) override;

	void Enter(EnemyObject* owner, float deltaTime) override;

	void Attack(EnemyObject* owner, float deltaTime);

private:
	Vector3 mAttackObjOffset;
};
