#pragma once
#include "GameObject.h"
#include "ShaderTag.h"

class ColliderComponent;
class SkeletalMeshComponent;
class Animation;
class EnemyStateBase;
class BoxCollider;
class LineCollider;

enum class EnemyState
{
	STATE_IDLE,

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

private:
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