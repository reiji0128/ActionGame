#pragma once
#include "PlayerStateBase.h"

class PlayerStateDamage : public PlayerStateBase
{
public:

	PlayerStateDamage();

	~PlayerStateDamage();

	PlayerState Update(PlayerObject* owner, float deltaTime) override;

	/// <summary>
	/// ��Ԃ��ڍs�����Ƃ���1�x�������鏈��
	/// </summary>
	/// <param name="owner">�v���C���[�̃|�C���^</param>
	/// <param name="deltaTime">1�t���[���̌o�ߎ���</param>
	void Enter(PlayerObject* owner, float deltaTime) override;

};