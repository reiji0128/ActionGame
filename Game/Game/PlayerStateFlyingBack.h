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
	// 後ろに行くスピード
	float mBackSpeed;

	// 後ろに行くスピードの初期値
	const float mInitBackSpeed;

	// 減速スピード
	float mDecelerationSpeed;

	// 減速スピードの初期値
	const float mInitDecelerationSpeed;

	Vector3 mPlayerToEnemyVec;

	Vector3 mOwnerPos;
};