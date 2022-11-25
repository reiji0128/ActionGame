#include "GameObject.h"
#include "Game.h"
#include "Component.h"

#include <algorithm>
#include <typeinfo>

// �Ǘ�ID�p�̐ÓI�J�E���^�̏�����
int GameObject::mGlobalObjectNo = 0;

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="objectTag">�I�u�W�F�N�g�̃^�O</param>
GameObject::GameObject(Tag objectTag)
	:mTag(objectTag)
	,mState(EActive)
	,mPosition(Vector3::Zero)
	,mDirection(Vector3::UnitX)
	,mRotation(Quaternion::Identity)
	,mScale(1.0f)
	,mRecomputeWorldTransform(true)
	,mID(mGlobalObjectNo)
	,mSpeed(0.0f)
{
	//�Q�[���V�X�e���{�̂ɃQ�[���I�u�W�F�N�g��ǉ�
	GAMEINSTANCE.AddGameObject(this);
	mGlobalObjectNo++;
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
GameObject::~GameObject()
{
	// �Q�[���I�u�W�F�N�g�������Ă���R���|�[�l���g�̍폜
	while (!mComponents.empty())
	{
		delete mComponents.back();
	}
	// �Q�[���V�X�e���{�̂ɂ��̃Q�[���I�u�W�F�N�g�̍폜���˗�
	GAMEINSTANCE.RemoveGameObject(this);
}

/// <summary>
/// �X�V����
/// </summary>
/// <param name="deltaTime">1�t���[���̌o�ߎ���</param>
void GameObject::Update(float deltaTime)
{
	// �Q�[���I�u�W�F�N�g�������Ă���Ƃ��̂ݏ���
	// ���[���h�s��̍Čv�Z�A�R���|�[�l���g�ƃQ�[���I�u�W�F�N�g�̍X�V�������s��
	if (mState == EActive)
	{
		ComputeWorldTransform();

		UpdateComponents(deltaTime);
		UpdateGameObject(deltaTime);

		ComputeWorldTransform();
	}
}

/// <summary>
/// �R���|�[�l���g�̍X�V����
/// </summary>
/// <param name="deltaTime">1�t���[���̌o�ߎ���</param>
void GameObject::UpdateComponents(float deltaTime)
{
	for (auto comp : mComponents)
	{
		comp->Update(deltaTime);
	}

	// ����ł���R���|�[�l���g���ꎞ�ۊ�
	for (auto comp : mComponents)
	{
		if (comp->GetState() == Component::EDelete)
		{
			mDeleteComponents.emplace_back(comp);
		}
	}

	// �����R���|�[�l���g��delete mComponents�����Delete�����
	for (auto comp : mDeleteComponents)
	{
		delete comp;
	}
	mDeleteComponents.clear();
}

/// <summary>
/// �Q�[���I�u�W�F�N�g��p�̍X�V����
/// </summary>
/// <param name="deltaTime">1�t���[���̌o�ߎ���</param>
void GameObject::UpdateGameObject(float deltaTime)
{
}

/// <summary>
/// ���͏���
/// </summary>
void GameObject::ProcessInput()
{
	if (mState == EActive)
	{
		// ���͏������󂯎��R���|�[�l���g��D�悵�Ď��s
		for (auto comp : mComponents)
		{
			comp->ProcessInput();
		}
	}
}

/// <summary>
/// �Փˏ���
/// </summary>
/// <param name="ownCollider">�Q�[���I�u�W�F�N�g�̏��L����ColliderComponent�̃|�C���^</param>
/// <param name="otherCollider">�Փˑ����ColliderComponent�̃|�C���^</param>
void GameObject::OnCollisionEnter(ColliderComponent* ownCollider, ColliderComponent* otherCollider)
{
}

/// <summary>
/// �O�i�x�N�g�������]���v�Z
/// </summary>
void GameObject::RotateToNewForward()
{
	// X���x�N�g��(1,0,0)��mDirection�̊Ԃ̊p�x�����߂�
	float dot = Vector3::Dot(Vector3::UnitX, mDirection);
	float angle = Math::Acos(dot);
	// �������������ꍇ
	if (dot > 0.9999f)
	{
		SetRotation(Quaternion::Identity);
	}
	// ������������ꍇ
	else if (dot < -0.9999f)
	{
		SetRotation(Quaternion(Vector3::UnitZ, Math::Pi));
	}
	else
	{
		// ���x�N�g����mDirection�Ƃ̊O�ς����]�������Ƃ߁A��]������
		Vector3 axis = Vector3::Cross(Vector3::UnitX, mDirection);
		axis.Normalize();
		SetRotation(Quaternion(axis, angle));
	}
}

/// <summary>
/// �O���x�N�g�������]���v�Z
/// </summary>
/// <param name="forward">�O���x�N�g��</param>
void GameObject::RotateToNewForward(const Vector3& forward)
{
	// X���x�N�g��(1,0,0)��mDirection�̊Ԃ̊p�x�����߂�
	float dot = Vector3::Dot(Vector3::UnitX, forward);
	float angle = Math::Acos(dot);

	// �������������ꍇ
	if (dot > 0.9999f)
	{
		SetRotation(Quaternion::Identity);
	}
	// ������������ꍇ
	else if (dot < -0.9999f)
	{
		SetRotation(Quaternion(Vector3::UnitZ, Math::Pi));
	}
	else
	{
		// ���x�N�g����mDirection�Ƃ̊O�ς����]�������Ƃ߁A��]������
		Vector3 axis = Vector3::Cross(Vector3::UnitX, forward);
		axis.Normalize();
		SetRotation(Quaternion(axis, angle));
	}
}

/// <summary>
/// ���[���h�ϊ��s��̌v�Z
/// </summary>
void GameObject::ComputeWorldTransform()
{
	//���[���h�ϊ��̍Čv�Z���K�v�Ȃ���s
	if (mRecomputeWorldTransform)
	{
		mRecomputeWorldTransform = false;

		//�O�i�x�N�g�������]���v�Z
		RotateToNewForward();

		// �X�P�[�����O����]�����s�ړ��ƂȂ�悤�ɕϊ��s����쐬
		mWorldTransform = Matrix4::CreateScale(mScale);
		mWorldTransform *= Matrix4::CreateFromQuaternion(mRotation);
		mWorldTransform *= Matrix4::CreateTranslation(mPosition);

		// �A�N�^�[�������Ă���S�R���|�[�l���g�̕ϊ��𑣂�
		for (auto comp : mComponents)
		{
			comp->OnUpdateWorldTransform();
		}
	}
}

/// <summary>
/// �R���|�[�l���g�̒ǉ�
/// </summary>
/// <param name="component">�ǉ�����Component�̃|�C���^</param>
void GameObject::AddComponent(Component* addComponent)
{
	// �R���|�[�l���g���\�[�g���Ēǉ�
	// �����̃I�[�_�[�ԍ������傫���}���_��������
	int myOrder = addComponent->GetUpdateOrder();
	auto iter = mComponents.begin();
	for (;
		iter != mComponents.end();
		++iter)
	{
		if (myOrder < (*iter)->GetUpdateOrder())
		{
			break;
		}
	}
	// �v�f���������|�C���g�̎�O�ɑ}������
	mComponents.insert(iter, addComponent);
}

/// <summary>
/// �R���|�[�l���g�̍폜 
/// </summary>
/// <param name="component">�폜����Component�̃|�C���^</param>
void GameObject::RemoveComponent(Component* removeComponent)
{
	auto iter = std::find(mComponents.begin(), mComponents.end(), removeComponent);
	if (iter != mComponents.end())
	{
		mComponents.erase(iter);
	}
}