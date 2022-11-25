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

	// ���̃R���{�Ɉڍs���邩�̃t���O
	bool IsNextCombo;

	// ���̃R���{�Ɉڍs���邽�߂̃^�C�}�[
	float NextComboTimer;

	// ���̃R���{�̃��~�b�g�^�C�}�[
	const float NextComboLimitTimer = 1.5f;

};