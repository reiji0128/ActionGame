#pragma once
#include "GameObject.h"
#include "Collision.h"

class AttackObj : public GameObject
{
public:
	AttackObj(Tag attackerType, const Vector3& pos, const Vector3& dir, float scale, float lifeTime = 1.0f);
	AttackObj(Tag attackerType, const Vector3& pos, const Vector3& offset, const Vector3& dir, float scale, float lifeTime = 1.0f);
	AttackObj(Tag attackerType, const Vector3& pos, const Vector3& dir, float scale, bool IsChangeScale,float incremental, float lifeTime = 1.0f);
	AttackObj(Tag attackerType, const Vector3& pos, const Vector3& dir, const AABB& hitRange, float lifeTime = 1.0f);

	void UpdateGameObject(float deltaTime)override;

private:
	// 寿命
	float mLifeTime;

	// 現在まで生きた時間
	float mAliveTime;

	// スケールの増分
	float mIncrementalScale;
	
	// スケールを変更するか
	bool mIsChangeScale;

};