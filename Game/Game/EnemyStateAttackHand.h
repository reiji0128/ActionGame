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

	// �U���A�j���J�n���Ă���U�����肪��������܂ł̒�������
	const float mAttackOffsetTime;

	// �~�܂�܂ł̎���
	const float mMoveStopTime;

	float mMoveStopTimer;
};