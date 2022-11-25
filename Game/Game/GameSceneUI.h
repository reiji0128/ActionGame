#pragma once

// 前方宣言
class HitPointBackGround;
class HitPointGauge;
class HitPointGaugeFrame;
class IHitPointGetter;
class GameClearSprite;
class GameOverSprite;
class CurtainSprite;

class GameSceneUI final
{
public:
	GameSceneUI(IHitPointGetter* playerHitPointGetter, IHitPointGetter* enemyHitPointGetter);

	~GameSceneUI();

	void Update(float deltaTime);

	void Draw();

	bool IsFinishFadeIn();

	bool IsFinishFadeOut();

private:

	// カウントタイム
	float mCountTime;

	// 遅延時間
	const float mDelayTime;

	// フェードインするか
	bool mIsFadeIn;

	// フェードアウトするか
	bool mIsFadeOut;

	IHitPointGetter* mPlayerHPGetter;

	IHitPointGetter* mEnemyHPGetter;

	// プレイヤーのHPゲージ関連
	HitPointBackGround* mPlayerHitPointBackGround;
	HitPointGauge* mPlayerHitPointGauge;
	HitPointGaugeFrame* mPlayerGaugeFrame;

	// エネミーのHPゲージ関連
	HitPointBackGround* mEnemyHitPointBackGround;
	HitPointGauge* mEnemyHitPointGauge;
	HitPointGaugeFrame* mEnemyGaugeFrame;

	// ゲームクリア
	GameClearSprite* mGameClear;

	// ゲームオーバー
	GameOverSprite* mGameOver;

	// 操作説明
	HitPointBackGround* mOperationExplanation;

	// フェードイン・フェードアウトの幕
	CurtainSprite* mCurtain;
};