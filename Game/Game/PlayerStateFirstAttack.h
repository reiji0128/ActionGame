#pragma once
#include "PlayerStateBase.h"

class PlayerObject;

class PlayerStateFirstAttack : public PlayerStateBase
{
public:

	PlayerStateFirstAttack();

	~PlayerStateFirstAttack();

	PlayerState Update(PlayerObject* owner, float deltaTime) override;

	void Enter(PlayerObject* owner, float deltaTime) override;

private:
	void Attack(PlayerObject* owner, float deltaTime);

};