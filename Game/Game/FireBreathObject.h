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
	/// ゲームオブジェクトの更新処理
	/// </summary>
	/// <param name="deltaTime"></param>
	void UpdateGameObject(float deltaTime) override;

	/// 衝突処理
	/// </summary>
	/// <param name="ownCollider">自分のコライダー</param>
	/// <param name="otherCollider">相手のコライダー</param>
	void OnCollisionEnter(ColliderComponent* ownCollider, ColliderComponent* otherCollider) override;

	void SetMouthPos(Vector3 mouthPos) { mMouthPos = mouthPos; }
private:
	Vector3 mMouthPos;

	BoxCollider* mBoxCollider;

	PointLightComponent* mPointLightComp;
};