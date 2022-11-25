#pragma once
#include "PlayerStateBase.h"

class PlayerStateFlyingBack : public PlayerStateBase
{
public:
	PlayerStateFlyingBack();

	~PlayerStateFlyingBack();

	PlayerState Update(PlayerObject* owner, float deltaTime) override;

	void Enter(PlayerObject* owner, float deltaTime) override;

private:
	// ���ɍs���X�s�[�h
	float mBackSpeed;

	// ���ɍs���X�s�[�h�̏����l
	const float mInitBackSpeed;

	// �����X�s�[�h
	float mDecelerationSpeed;

	// �����X�s�[�h�̏����l
	const float mInitDecelerationSpeed;

	Vector3 mPlayerToEnemyVec;

	Vector3 mOwnerPos;
};