#pragma once
#include "CameraObject.h"

class ThirdPersonCameraObject : public CameraObject
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	ThirdPersonCameraObject(GameObject* targetObject);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~ThirdPersonCameraObject();

	/// <summary>
	/// �A�N�^�[�̍X�V����
	/// </summary>
	/// <param name="deltaTime">1�t���[���̌o�ߎ���</param>
	void UpdateGameObject(float deltaTime) override;

// �Z�b�^�[ //

	/// <summary>
	/// Z���̉�]�p�̃Z�b�g
	/// </summary>
	/// <param name="angle">Z���̉�]�p</param>
	void SetRotateZAngle(float angle) { mRotateZAngle = angle; }

	/// <summary>
	/// �����낵�̉�]�p�̃Z�b�g
	/// </summary>
	/// <param name="angle">�����낵�̉�]�p</param>
	void SetLookDownAngle(float angle) { mLookDownAngle = angle; }

	/// <summary>
	/// �J�����̋����̃Z�b�g
	/// </summary>
	/// <param name="length">�J�����̋���</param>
	void SetCameraLength(float length) { mCameraLength = length; }

	/// <summary>
	/// �J�������W�̃Z�b�g
	/// </summary>
	/// <param name="vec">�J�������W</param>
	void SetCameraPos(Vector3& vec) { mCameraPos = vec; }

// �Q�b�^�[ //

	/// <summary>
	/// �J�����̑O���x�N�g���̎擾
	/// </summary>
	/// <returns>�J�����̑O���x�N�g��</returns>
	Vector3& GetCameraForwardVec() { return mCameraForwardVec; }

	/// <summary>
	/// �J�����̍��W�̎擾
	/// </summary>
	/// <returns>�J�����̍��W</returns>
	Vector3& GetCameraPos() { return mCameraPos; }

private:
	// �v���C���[����̋���
	float mCameraLength;

	// �J�������͉�]������]�p
	float mRotateZAngle;

	// �J���������낵������]�p
	float mLookDownAngle;

	// �O���x�N�g��
	Vector3 mCameraForwardVec;

	// �J�����ʒu
	Vector3 mCameraPos;

	Vector3 mTargetDir;
};
