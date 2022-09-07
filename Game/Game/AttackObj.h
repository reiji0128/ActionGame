#pragma once
#include "GameObject.h"
#include "Collision.h"

class AttackObj : public GameObject
{
public:
	AttackObj(Tag attackerType, const Vector3& pos, const Vector3& dir, float scale, float lifeTime = 1.0f);
	AttackObj(Tag attackerType, const Vector3& pos, const Vector3& dir, const AABB& hitRange, float lifeTime = 1.0f);

	void UpdateGameObject(float deltaTime)override;

private:
	float mLifeTime  ; // 寿命
	float mAliveTime ; // 現在まで生きた時間

};