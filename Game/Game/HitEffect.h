#pragma once
#include "GameObject.h"

// 前方宣言
class EffectComponent;

class HitEffect : public GameObject
{
public:
	HitEffect(GameObject* owner,const Vector3& offset);

	~HitEffect();

	/// <summary>
	/// ゲームオブジェクトの更新処理
	/// </summary>
	/// <param name="deltaTime"></param>
	void UpdateGameObject(float deltaTime) override;

	void PlayEffect();

private:
	Vector3 mOffset;

	GameObject* mOwner;

	EffectComponent* mEffectComp;
};
