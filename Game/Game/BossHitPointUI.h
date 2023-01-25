#pragma once

// 前方宣言
class Health;
class HitPointBackGround;
class HitPointGauge;
class HitPointGaugeFrame;

class BossHitPointUI
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="playerHealth">ボスの体力</param>
	BossHitPointUI(Health* bossHealth);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~BossHitPointUI();

private:

	/// <summary>
	/// 体力の変更があったときに呼ばれる関数
	/// </summary>
	void OnHealthChanged();

	// ボスの体力
	Health* mBossHealth;

	// 体力ゲージの背景
	HitPointBackGround* mBackGround;

	// 体力ゲージ
	HitPointGauge* mGauge;

	// 体力ゲージのフレーム
	HitPointGaugeFrame* mFrame;
};