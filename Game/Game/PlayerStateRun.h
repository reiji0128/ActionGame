#pragma once
#include "PlayerStateBase.h"
#include "ThirdPersonCameraObject.h"

class PlayerObject;

class PlayerStateRun : public PlayerStateBase
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	PlayerStateRun();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~PlayerStateRun();

	/// <summary>
	/// �X�V����
	/// </summary>
	/// <param name="owner">�v���C���[�̃|�C���^</param>
	/// <param name="deltaTime">1�t���[���̌o�ߎ���</param>
	/// <returns></returns>
	PlayerState Update(PlayerObject* owner, float deltaTime) override;

	/// <summary>
	/// ��Ԃ��ڍs�����Ƃ���1�x�������鏈��
	/// </summary>
	/// <param name="owner">�v���C���[�̃|�C���^</param>
	/// <param name="deltaTime">1�t���[���̌o�ߎ���</param>
	void Enter(PlayerObject* owner, float deltaTime) override;

private:
	/// <summary>
	/// �ړ�����
	/// </summary>
	/// <param name="owner">�v���C���[�̃|�C���^</param>
	/// <param name="deltaTime">1�t���[���̌o�ߎ���</param>
	void MoveCalc(PlayerObject* owner, float deltaTime);

	/// <summary>
	/// ��]����
	/// </summary>
	void RotateActor();

	// �v���C���[�̑O���x�N�g��
	Vector3 mCharaForwardVec;

	// �ڕW��]����
	Vector3 mRotationTargetDir;

	// ��]����
	bool mRotateNow;
};