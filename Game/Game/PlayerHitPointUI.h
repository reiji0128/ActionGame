#pragma once

// 前方宣言
class Health;
class HitPointBackGround;
class HitPointGauge;
class HitPointGaugeFrame;

class PlayerHitPointUI
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="playerHealth">プレイヤーの体力</param>
	PlayerHitPointUI(Health* playerHealth);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~PlayerHitPointUI();

private:

	/// <summary>
	/// 体力の変更があったときに呼ばれる関数
	/// </summary>
	void OnHealthChanged();

	// プレイヤーの体力
	Health* mPlayerHealth;

	// 体力ゲージの背景
	HitPointBackGround* mBackGround;

	// 体力ゲージ
	HitPointGauge* mGauge;

	// 体力ゲージのフレーム
	HitPointGaugeFrame* mFrame;
};
