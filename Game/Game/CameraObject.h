#pragma once
#include "GameObject.h"

class CameraObject : public GameObject
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="targetActor">�J��������Ƃ���Q�[���I�u�W�F�N�g�̃|�C���^</param>
	CameraObject(GameObject* targetActor);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~CameraObject();

	/// <summary>
	/// �X�V����
	/// </summary>
	/// <param name="deltaTime">1�t���[���̌o�ߎ���</param>
	void Update(float deltaTime);

	/// <summary>
	/// ����������
	/// </summary>
	/// <param name="cameraPos">�J�����̍��W</param>
	/// <param name="targetPos">�^�[�Q�b�g�̍��W</param>
	/// <param name="upVec">�c���̃x�N�g��</param>
	void Init(const Vector3& cameraPos, const Vector3& targetPos,
		      const Vector3& upVec);
	
// �Z�b�^�[ //

	/// <summary>
	/// �A�N�e�B�u�ɃZ�b�g
	/// </summary>
	void SetActive();

// �Q�b�^�[ //

	/// <summary>
	/// �r���[�s��̎擾
	/// </summary>
	/// <returns>�r���[�s��</returns>
	Matrix4& GetViewMatrix() { return mViewMatrix; }

	/// <summary>
	/// �����x�N�g���̎擾
	/// </summary>
	/// <returns>�����x�N�g��</returns>
	Vector3& GetViewVector() { return mViewVector; }

	/// <summary>
	/// �r���[�^�[�Q�b�g�̍��W�̎擾
	/// </summary>
	/// <returns>�r���[�^�[�Q�b�g�̍��W</returns>
	Vector3& GetViewTarget() { return mViewTarget; }

	/// <summary>
	/// �J�����̍��W�̎擾
	/// </summary>
	/// <returns>�J�����̍��W</returns>
	Vector3& GetViewPos() { return mPosition; }

protected:

	// �J��������Ƃ���Q�[���I�u�W�F�N�g�̃|�C���^
	GameObject* mTargetActor;

	// �r���[�s��
	Matrix4 mViewMatrix;

	// �J���������_
	Vector3 mViewTarget;

	// �����x�N�g��
	Vector3 mViewVector;
};