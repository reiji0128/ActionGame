#pragma once
#include "EnemyStaeBase.h"

class EnemyStateDetection : public EnemyStateBase
{
public:
	EnemyStateDetection();

	~EnemyStateDetection();

	EnemyState Update(EnemyObject* owner, float deltaTime) override;

	void Enter(EnemyObject* owner, float deltaTime) override;

private:

	void MoveCalc(EnemyObject* owner, float deltaTime,const Vector3& targetDir);

	float mRunSpeed;
};