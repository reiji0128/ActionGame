#pragma once
#include "GameObject.h"

// �O���錾
class PointLightComponent;
class BoxCollider;


class FireBallObject : public GameObject 
{
public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="offset">���W</param>
	/// <param name="objectTag">�I�u�W�F�N�g�̃^�O</param>
	FireBallObject(const Vector3& pos,const Vector3& dir,const float& speed,Tag objectTag);

	
	~FireBallObject();

	/// <summary>
	/// �Q�[���I�u�W�F�N�g�̍X�V����
	/// </summary>
	/// <param name="deltaTime"></param>
	void UpdateGameObject(float deltaTime) override;

	/// �Փˏ���
	/// </summary>
	/// <param name="ownCollider">�����̃R���C�_�[</param>
	/// <param name="otherCollider">����̃R���C�_�[</param>
	void OnCollisionEnter(ColliderComponent* ownCollider, ColliderComponent* otherCollider) override;

private:

	// �I�t�Z�b�g
	const Vector3 mOffset;

	BoxCollider* mBoxCollider;

	PointLightComponent* mPointLightComp;
};