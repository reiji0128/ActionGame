#pragma once
#include "PlayerStateBase.h"

class PlayerObject;

class PlayerStateIdle : public PlayerStateBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	PlayerStateIdle();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~PlayerStateIdle();

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
};