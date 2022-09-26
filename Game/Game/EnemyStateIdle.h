#pragma once
#include "EnemyStaeBase.h"

class EnemyObject;

class EnemyStateIdle : public EnemyStateBase
{
public:
	EnemyStateIdle();

	~EnemyStateIdle();

	EnemyState Update(EnemyObject* owner, float deltaTime) override;

	void Enter(EnemyObject* owner, float deltaTime) override;

private:
	// �v���C���[�Ƃ̋���
	const float mNearLength = 250.0f;

	// �ҋ@����
	const float mWaitTime = 2.0f;

};