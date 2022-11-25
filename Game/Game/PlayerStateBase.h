#pragma once
#include "PlayerObject.h"

class PlayerObject;
class SkeletalMeshComponent;

class PlayerStateBase
{
public:

	PlayerStateBase() {};

	virtual ~PlayerStateBase() {};

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="owner">プレイヤーのポインタ</param>
	/// <param name="deltaTime">1フレームの経過時間</param>
	/// <returns>プレイヤーの状態</returns>
	virtual PlayerState Update(PlayerObject* owner, float deltaTime) = 0;

	/// <summary>
	/// 状態が移行したときに1度だけする処理
	/// </summary>
	/// <param name="owner">プレイヤーのポインタ</param>
	/// <param name="deltaTime">1フレームの経過時間</param>
	virtual void Enter(PlayerObject* owner, float deltaTime) {};

	/// <summary>
	/// 入力処理
	/// </summary>
	virtual void InputProcess() {};

protected:
	// ステートのタイマー
	float mStateTimer;

	// コントローラーの入力がなかったか
	bool IsControllerInputOff;

	// アイドル状態か
	bool IsIdle;

	// 攻撃フラグ
	bool IsAttack;

	bool IsRollForward;

	// スケルタルメッシュクラスのポインタ
	SkeletalMeshComponent* mSkelComp;
};