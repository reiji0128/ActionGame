#pragma once
#include <vector>
#include "Math.h"
#include "Tag.h"

// �O���錾
class Component;
class ColliderComponent;

class GameObject
{
public:

	/// <summary>
	/// �Q�[���I�u�W�F�N�g�̏��
	/// </summary>
	enum State
	{
		EActive,      // �A�N�e�B�u���
		EInactive,    // ��A�N�e�B�u���
		EPaused,      // �|�[�Y���
		EDead         // ���S���
	};

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="objectTag">�I�u�W�F�N�g�̃^�O</param>
	GameObject(Tag objectTag);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~GameObject();

	/// <summary>
	/// �X�V����
	/// </summary>
	/// <param name="deltaTime">1�t���[���̌o�ߎ���</param>
	void Update(float deltaTime);

	/// <summary>
	/// ���L����R���|�[�l���g�̍X�V����
	/// </summary>
	/// <param name="deltaTime">1�t���[���̌o�ߎ���</param>
	void UpdateComponents(float deltaTime);

	/// <summary>
	/// �Q�[���I�u�W�F�N�g��p�̍X�V����
	/// </summary>
	/// <param name="deltaTime"></param>
	virtual void UpdateGameObject(float deltaTime);

	/// <summary>
	/// ���͏���
	/// </summary>
	void ProcessInput();

	/// <summary>
	/// �Փˏ���(���̕��̂Ɠ��������Ƃ��ɌĂ΂��)
	/// </summary>
	/// <param name="ownCollider">�Q�[���I�u�W�F�N�g�̏��L����ColliderComponent�̃|�C���^</param>
	/// <param name="otherCollider">�Փˑ����ColliderComponent�̃|�C���^</param>
	virtual void OnCollisionEnter(ColliderComponent* ownCollider, ColliderComponent* otherCollider);

	/// <summary>
	/// �O�i�x�N�g�������]���v�Z
	/// </summary>
	void RotateToNewForward();

	/// <summary>
	/// �O�i�x�N�g�������]���v�Z
	/// </summary>
	/// <param name="forward">�O���x�N�g��</param>
	void RotateToNewForward(const Vector3& forward);

	/// <summary>
	/// ���[���h�ϊ��s��̌v�Z
	/// </summary>
	void ComputeWorldTransform();

	/// <summary>
	/// �R���|�[�l���g�̒ǉ�
	/// </summary>
	/// <param name="addComponent">�ǉ�����Component�̃|�C���^</param>
	void AddComponent(Component* addComponent);

	/// <summary>
	/// �R���|�[�l���g�̍폜 
	/// </summary>
	/// <param name="removeComponent">�폜����Component�̃|�C���^</param>
	void RemoveComponent(Component* removeComponent);

// �Z�b�^�[ //

	/// <summary>
	/// �ʒu�̃Z�b�g
	/// </summary>
	/// <param name="pos">�Z�b�g������W</param>
	void SetPosition(const Vector3& pos) { mPosition = pos; mRecomputeWorldTransform = true; }

	/// <summary>
	/// �X�P�[���̃Z�b�g(float�p)
	/// </summary>
	/// <param name="scale">�X�P�[���{��</param>
	void SetScale(float scale) { mScale = scale; mRecomputeWorldTransform = true; }

	/// <summary>
	/// ��](�N�H�[�^�j�I��)�̃Z�b�g
	/// </summary>
	/// <param name="rotation">�Z�b�g�����]�p</param>
	void SetRotation(const Quaternion& rotation) { mRotation = rotation;  mRecomputeWorldTransform = true; }

	/// <summary>
	/// �����̃Z�b�g
	/// </summary>
	/// <param name="dir">�Z�b�g�������</param>
	void SetDirection(const Vector3& dir) { mDirection = dir; }

	/// <summary>
	/// ���[���h�ϊ����������邩�H (�ړ��ȂǕϊ��s��̍Čv�Z���K�v�Ȏ�)
	/// </summary>
	void SetComputeWorldTransform() { mRecomputeWorldTransform = true; }

	/// <summary>
	/// �Q�[���I�u�W�F�N�g�̃X�s�[�h�̃Z�b�g
	/// </summary>
	/// <param name="speed">�Z�b�g����X�s�[�h</param>
	void SetSpeed(float speed) { mSpeed = speed; }

	/// <summary>
	/// �Q�[���I�u�W�F�N�g�̏�Ԃ̃Z�b�g
	/// </summary>
	/// <param name="state">�Z�b�g������</param>
	void SetState(State state) { mState = state; }

// �Q�b�^�[ //

	/// <summary>
	/// �ʒu�̎擾
	/// </summary>
	/// <returns>Actor�̈ʒu</returns>
	const Vector3& GetPosition() const { return mPosition; }

	/// <summary>
	/// �X�P�[���̎擾
	/// </summary>
	/// <returns>Actor�̃X�P�[��</returns>
	const float& GetScale() const { return mScale; }

	/// <summary>
	/// ��](�N�H�[�^�j�I��)�̎擾
	/// </summary>
	/// <returns>��]�p</returns>
	const Quaternion& GetRotation() const { return mRotation; }

	/// <summary>
	/// �����̎擾
	/// </summary>
	/// <returns>Actor�̌���</returns>
	const Vector3& GetDirection() const { return mDirection; }

	/// <summary>
	/// �^�O�̎擾
	/// </summary>
	/// <returns>�^�O�̎��</returns>
	Tag GetTag() { return mTag; }

	/// <summary>
	/// ���[���h�ϊ��s��̎擾
	/// </summary>
	/// <returns>���[���h�ϊ��s��</returns>
	const Matrix4& GetWorldTransform() const { return mWorldTransform; }

	/// <summary>
	/// �O���x�N�g���̎擾
	/// </summary>
	/// <returns>�O���x�N�g��</returns>
	Vector3 GetForward() const { return Vector3::Transform(Vector3::UnitX, mRotation); }

	/// <summary>
	/// �E�����x�N�g���̎擾
	/// </summary>
	/// <returns>�E�����x�N�g��</returns>
	Vector3 GetRight() const { return Vector3::Transform(Vector3::UnitY, mRotation); }

	/// <summary>
	/// ��Ԃ̎擾
	/// </summary>
	/// <returns>Actor�̏��</returns>
	State GetState() const { return mState; }

	/// <summary>
	/// �X�s�[�h�̎擾
	/// </summary>
	/// <returns>Actor�̃X�s�[�h</returns>
	float GetSpeed() const { return mSpeed; }

	/// <summary>
	/// �Q�[���I�u�W�F�N�g�̊Ǘ�ID�̎擾
	/// </summary>
	/// <returns>Actor�̊Ǘ�ID</returns>
	int GetID() { return mID; };

protected:
	// �ʒu
	Vector3 mPosition;

	// ����
	Vector3 mDirection;

	// ��]
	Quaternion mRotation;

	// �X�P�[��
	float mScale;

	// �X�s�[�h
	float mSpeed;

	// �Q�[���I�u�W�F�N�g�̃^�O
	Tag mTag;

	// �Q�[���I�u�W�F�N�g�̏��
	State mState;

	// ���[���h�ϊ��s��
	Matrix4 mWorldTransform;

	// ���[���h�ϊ��̍Čv�Z���K�v���H
	bool mRecomputeWorldTransform;

	// �Ǘ�ID
	int mID;

	// �Ǘ�ID�p�̐ÓI�J�E���^
	static int mGlobalObjectNo;

	// ���L����R���|�[�l���g���i�[����z��
	std::vector<class Component*> mComponents;

	// �폜����R���|�[�l���g���i�[����z��
	std::vector<class Component*> mDeleteComponents;
};
