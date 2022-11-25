#pragma once
#include "PlayerStateBase.h"

class PlayerObject;

class PlayerStateSecondAttack : public PlayerStateBase
{
public:
	PlayerStateSecondAttack();

	~PlayerStateSecondAttack();

	PlayerState Update(PlayerObject* owner, float deltaTime) override;

	void Enter(PlayerObject* owner, float deltaTime) override;

private:
	void Attack(PlayerObject* owner, float deltaTime);

	void InputProcess() override;
};