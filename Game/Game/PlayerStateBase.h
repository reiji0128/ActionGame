#pragma once
#include "PlayerObject.h"

class PlayerObject;
class SkeletalMeshComponent;

class PlayerStateBase
{
public:

	PlayerStateBase() {};

	virtual ~PlayerStateBase() {};

	/// <summary>
	/// �X�V����
	/// </summary>
	/// <param name="owner">�v���C���[�̃|�C���^</param>
	/// <param name="deltaTime">1�t���[���̌o�ߎ���</param>
	/// <returns>�v���C���[�̏��</returns>
	virtual PlayerState Update(class PlayerObject* owner, float deltaTime) = 0;

	/// <summary>
	/// ��Ԃ��ڍs�����Ƃ���1�x�������鏈��
	/// </summary>
	/// <param name="owner">�v���C���[�̃|�C���^</param>
	/// <param name="deltaTime">1�t���[���̌o�ߎ���</param>
	virtual void Enter(class PlayerObject* owner, float deltaTime) {};

protected:
	// �X�e�[�g�̃^�C�}�[
	float mStateTimer;

	bool IsFirstAttack;

	// �X�P���^�����b�V���N���X�̃|�C���^
	SkeletalMeshComponent* mSkelComp;
};