#pragma once
#include "GameObject.h"

class PointLightComponent;
class BoxCollider;

class FireBreathObject : public GameObject
{
public:

	FireBreathObject(const Vector3& offset, const Vector3& dir, Tag objectTag);

	~FireBreathObject();

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

	void SetMouthPos(Vector3 mouthPos) { mMouthPos = mouthPos; }
private:
	Vector3 mMouthPos;

	BoxCollider* mBoxCollider;

	PointLightComponent* mPointLightComp;
};