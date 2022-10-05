#pragma once
#include "EnemyObject.h"

class EnemyObject;
class SkeletalMeshComponent;

class EnemyStateBase
{
public:
	EnemyStateBase();

	virtual ~EnemyStateBase() {};

	virtual EnemyState Update(EnemyObject* owner, float deltaTime) = 0;

	virtual void Enter(EnemyObject* owner, float deltaTime) {};

protected:
	// ステートのタイマー
	float mStateTimer;

	// 攻撃中から追跡に変わる距離
	const float retrackingRange = 300.0f;

	// スケルタルメッシュクラスのポインタ
	SkeletalMeshComponent* mSkelComp;
 };