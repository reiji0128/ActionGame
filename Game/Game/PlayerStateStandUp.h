#pragma once
#include "PlayerStateBase.h"

class PlayerStateStandUp : public PlayerStateBase
{
public:

	PlayerStateStandUp();

	~PlayerStateStandUp();

	PlayerState Update(PlayerObject* owner, float deltaTime) override;

	void Enter(PlayerObject* owner, float deltaTime) override;
};