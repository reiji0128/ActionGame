#pragma once
#include "PlayerStateBase.h"

class PlayerStateDamage : public PlayerStateBase
{
public:

	PlayerStateDamage();

	~PlayerStateDamage();

	PlayerState Update(PlayerObject* owner, float deltaTime) override;

	/// <summary>
	/// 状態が移行したときに1度だけする処理
	/// </summary>
	/// <param name="owner">プレイヤーのポインタ</param>
	/// <param name="deltaTime">1フレームの経過時間</param>
	void Enter(PlayerObject* owner, float deltaTime) override;

};