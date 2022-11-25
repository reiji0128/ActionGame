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

	void Attack(PlayerObject* owner, float deltaTime);
private:

	void InputProcess() override;

	// 次のコンボに移行するかのフラグ
	bool IsNextCombo;

	// 次のコンボに移行するためのタイマー
	float NextComboTimer;

	// 次のコンボのリミットタイマー
	const float NextComboLimitTimer = 1.5f;

};