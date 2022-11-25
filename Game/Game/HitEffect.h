#pragma once
#include "GameObject.h"

// �O���錾
class EffectComponent;

class HitEffect : public GameObject
{
public:
	HitEffect(GameObject* owner,const Vector3& offset);

	~HitEffect();

	/// <summary>
	/// �Q�[���I�u�W�F�N�g�̍X�V����
	/// </summary>
	/// <param name="deltaTime"></param>
	void UpdateGameObject(float deltaTime) override;

	void PlayEffect();

private:
	Vector3 mOffset;

	GameObject* mOwner;

	EffectComponent* mEffectComp;
};
