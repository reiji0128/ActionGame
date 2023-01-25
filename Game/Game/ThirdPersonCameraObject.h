#pragma once
#include "CameraObject.h"

class TargetSearch;

class ThirdPersonCameraObject : public CameraObject
{
public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	ThirdPersonCameraObject(GameObject* targetObject, TargetSearch* targetSearch);

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

	void TargetLockOn();

	// �v���C���[����̋���
	float mCameraLength;

	// �J�������͉�]������]�p
	float mRotateZAngle;

	// �J���������낵������]�p
	float mLookDownAngle;

	// �߂荞�ݖh�~�̕␳�l
	const float mCorrectionValue;

	float mTime;

	// �O���x�N�g��
	Vector3 mCameraForwardVec;

	// �J�����ʒu
	Vector3 mCameraPos;

	Vector3 mPlayerDir;

	// �^�[�Q�b�g���b�N�����Ă��邩
	bool mIsTargetLocke;

	// ��Ԃ��I�������
	bool mIsFinishInterpolation;

	// �^�[�Q�b�g��T���N���X
	TargetSearch* mTargetSearch;
};
