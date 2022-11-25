#pragma once
#include "PlayerStateBase.h"

class PlayerStateRollForward : public PlayerStateBase
{
public:
	PlayerStateRollForward();
	~PlayerStateRollForward();

	PlayerState Update(PlayerObject* owner, float deltaTime) override;

	/// <summary>
	/// ��Ԃ��ڍs�����Ƃ���1�x�������鏈��
	/// </summary>
	/// <param name="owner">�v���C���[�̃|�C���^</param>
	/// <param name="deltaTime">1�t���[���̌o�ߎ���</param>
	void Enter(PlayerObject* owner, float deltaTime) override;

private:
	void RollCalc(PlayerObject* owner, float deltaTime);

	// �X�s�[�h
	const float mSpeed;

	Vector3 mCharaForwardVec;
	
	Vector3 mOwnerPos;
};
