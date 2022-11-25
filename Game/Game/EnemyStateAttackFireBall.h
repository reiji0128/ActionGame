#pragma once
#include "EnemyStaeBase.h"

class EnemyStateAttackFireBall : public EnemyStateBase
{
public:
	
	EnemyStateAttackFireBall();

	~EnemyStateAttackFireBall();

	EnemyState Update(EnemyObject* owner, float deltaTime) override;

	void Enter(EnemyObject* owner, float deltaTime) override;

	void AttackProcess(EnemyObject* owner, float deltaTime);

private:

	// �G�l�~�[����v���C���[�Ɍ������x�N�g��
	Vector3 mEnemyToPlayerVec;

	// �G�l�~�[�̌��̍��W
	Vector3 mMouthPos;
};