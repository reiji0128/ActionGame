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

	/// <summary>
	/// 回転処理
	/// </summary>
	/// <param name="owner">エネミーのポインタ</param>
	/// <param name="deltaTime">1フレームの経過時間</param>
	void RotateProcess(EnemyObject* owner, float deltaTime);

	/// 視野角に入っているかどうか
	/// </summary>
	/// <param name="from">基準のベクトル</param>
	/// <param name="to">角度差を測定するベクトル</param>
	/// <param name="length">範囲の長さ</param>
	/// <returns>
	/// true  : 視野角に入っている
	/// false : 視野角に入っていない
	/// </returns> 
	bool InViewAngle(const Vector3& from, const Vector3& to, float length);

	// ステートのタイマー
	float mStateTimer;

	// 攻撃中から追跡に変わる距離
	const float retrackingRange = 600.0f;

	// プレイヤーとの距離(近距離)
	const float mNearLength = 600.0f;

	// プレイヤーとの距離(遠距離)
	const float mFarLength = 800.0f;

	// 視野角
	const float viewAngle = 45;

	// 攻撃アニメ開始してから攻撃判定が発生するまでの調整時間
	const float attackOffsetTime = 0.5f;

	// 回転スピード
	const float mRotateSpeed = 45.0f;

	// 回転しているか
	bool mRotateNow;

	// 目標へのベクトル
	Vector3 mAimVec;

	// スケルタルメッシュクラスのポインタ
	SkeletalMeshComponent* mSkelComp;
 };