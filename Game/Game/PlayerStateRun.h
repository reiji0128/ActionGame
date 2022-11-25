#pragma once
#include "PlayerStateBase.h"
#include "ThirdPersonCameraObject.h"

class PlayerObject;

class PlayerStateRun : public PlayerStateBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	PlayerStateRun();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~PlayerStateRun();

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="owner">プレイヤーのポインタ</param>
	/// <param name="deltaTime">1フレームの経過時間</param>
	/// <returns></returns>
	PlayerState Update(PlayerObject* owner, float deltaTime) override;

	/// <summary>
	/// 状態が移行したときに1度だけする処理
	/// </summary>
	/// <param name="owner">プレイヤーのポインタ</param>
	/// <param name="deltaTime">1フレームの経過時間</param>
	void Enter(PlayerObject* owner, float deltaTime) override;

private:

	/// <summary>
	/// 入力処理
	/// </summary>
	void InputProcess() override;

	/// <summary>
	/// 移動処理
	/// </summary>
	/// <param name="owner">プレイヤーのポインタ</param>
	/// <param name="deltaTime">1フレームの経過時間</param>
	void MoveCalc(PlayerObject* owner, float deltaTime);

	/// <summary>
	/// 回転処理
	/// </summary>
	void RotateActor();

	// プレイヤーの前方ベクトル
	Vector3 mCharaForwardVec;

	// 目標回転方向
	Vector3 mRotationTargetDir;

	// 回転中か
	bool mRotateNow;
};