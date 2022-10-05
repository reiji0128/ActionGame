#pragma once
#include "PlayerStateBase.h"

class PlayerStateDeath : public PlayerStateBase
{
public:
	PlayerStateDeath();

	~PlayerStateDeath();

	PlayerState Update(PlayerObject* owner, float deltaTime) override;

	void Enter(PlayerObject* owner, float deltaTime) override;
};
