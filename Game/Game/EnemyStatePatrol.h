#pragma once
#include "EnemyStaeBase.h"

class EnemyObject;

class EnemyStatePatrol : public EnemyStateBase
{
public:
	EnemyStatePatrol();

	~EnemyStatePatrol();

	EnemyState Update(EnemyObject* owner, float deltaTime) override;

	void Enter(EnemyObject* owner, float deltaTime) override;

private:
	const float mPatrolSpeed = 100.0f;   //�p�g���[�����̃X�s�[�h
	const float mRotateSpeed = 45.0f;    // ��]�X�s�[�h
	const float mTrackingRange = 500.0f; // �ǐՋ���

	Vector3 mEnemyToPlayerVec;			// �G�l�~�[����v���C���[�Ɍ������x�N�g��
	Vector3 mAimVec;                    // �ڕW��]����
	bool mRotateNow;                    // ��]�����H
};