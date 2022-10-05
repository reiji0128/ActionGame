#pragma once
#include "GameObject.h"
#include <functional>
#include "ShaderTag.h"

// 前方宣言
class ColliderComponent;
class SkeletalMeshComponent;
class Animation;
class EnemyStateBase;
class BoxCollider;
class LineCollider;

enum class EnemyState
{
	STATE_IDLE,
	STATE_PATROL,
	STATE_RUN,
	STATE_ATTACK_HAND,
	STATE_ATTACK_BITE,
	STATE_FIRE_BREATH,
	STATE_FIRE_BALL,
	STATE_DEATH,

	STATE_NUM
};

class EnemyObject : public GameObject
{
public:

	EnemyObject(const Vector3& pos, const float& scale, const char* gpmeshFileName, const char* gpskelFileName);

	~EnemyObject();

	void UpdateGameObject(float deltaTime) override;

	void OnCollisionEnter(ColliderComponent* ownCollider, ColliderComponent* otherCollider) override;

	SkeletalMeshComponent* GetSkeletalMeshComp() { return mSkeltalMeshComp; }

	const Animation* GetAnim(EnemyState state) const { return mAnimations[static_cast<unsigned int>(state)]; }

// セッター //
	void SetVelosity(const Vector3& velosity) { mVelocity = velosity; }

private:
	int mHP;

	const int mMaxHP = 100;

	Vector3 mVelocity;

	EnemyState mNowState;

	EnemyState mNextState;

	std::vector<const Animation*> mAnimations;

	std::vector<EnemyStateBase*> mStatePools;

	SkeletalMeshComponent* mSkeltalMeshComp;

	BoxCollider* mBoxCollider;

	LineCollider* mLineCollider;

	ShaderTag mUseShaderTag;
};