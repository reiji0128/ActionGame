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

	// �U��������ǐՂɕς�鋗��
	const float retrackingRange = 300.0f;
	// �U���A�j���J�n���Ă���U�����肪��������܂ł̒�������
	const float attackOffsetTime = 0.5f;
};