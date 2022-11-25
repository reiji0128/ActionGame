#include "GameSceneUI.h"
#include "Game.h"
#include "AudioManager.h"
#include "IHitPointGetter.h"
#include "HitPointBackGround.h"
#include "HitPointGauge.h"
#include "HitPointGaugeFrame.h"
#include "GameClearSprite.h"
#include "GameOverSprite.h"
#include "CurtainSprite.h"

GameSceneUI::GameSceneUI(IHitPointGetter* playerHitPointGetter, IHitPointGetter* enemyHitPointGetter)
	:mPlayerHPGetter(playerHitPointGetter)
	,mEnemyHPGetter(enemyHitPointGetter)
	,mCountTime(0.0f)
	,mDelayTime(1.0f)
	,mIsFadeIn(true)
	,mIsFadeOut(false)
{
	// プレイヤーのHPゲージ関連 //
	mPlayerHitPointBackGround = new HitPointBackGround(Vector2(390, 115),                  // 相対座標
		                                               Vector2(0.82, 1.0),                 // 拡大率
		                                               0.5,                                // 透過度
		                                               "Assets/UI/Player/PlayerBack.png"); // テクスチャのファイルパス

	mPlayerHitPointGauge = new HitPointGauge(Vector2(100, 100),                            // 相対座標
		                                     Vector2(1, 0.03f),                            // 拡大率
		                                     1.0,                                          // 透過度
		                                     "Assets/UI/Player/Health.png",                // テクスチャのファイルパス
		                                     playerHitPointGetter);                        // インターフェイス

	mPlayerGaugeFrame = new HitPointGaugeFrame(Vector2(392, 130),                          // 相対座標
		                                       Vector2(0.82, 2.0), 					       // 拡大率
		                                       1.0,									       // 透過度
		                                       "Assets/UI/Player/PlayerFrame.png");	       // テクスチャのファイルパス

// エネミーのHPゲージ関連 //
    mEnemyHitPointBackGround = new HitPointBackGround(Vector2(944, 900),                   // 相対座標
		                                              Vector2(0.82, 0.9),                  // 拡大率
		                                              0.5,                                 // 透過度
		                                              "Assets/UI/Enemy/EnemyBack.png");    // テクスチャのファイルパス

	mEnemyHitPointGauge = new HitPointGauge(Vector2(500, 900),                             // 相対座標
		                                    Vector2(1.6, 0.025f),                          // 拡大率
		                                    1.0,                                           // 透過度
		                                    "Assets/UI/Enemy/Health.png",                  // テクスチャのファイルパス
		                                    enemyHitPointGetter);                          // インターフェイス

	mEnemyGaugeFrame = new HitPointGaugeFrame(Vector2(950, 900),                           // 相対座標
		                                      Vector2(0.81, 2.0),                          // 拡大率
		                                      1.0,                                         // 透過度
		                                      "Assets/UI/Enemy/EnemyFrame.png");           // テクスチャのファイルパス

	mGameClear = new GameClearSprite(Vector2(960, 540),
		                             Vector2(1, 1),
		                             0.0f);

	mGameOver = new GameOverSprite(Vector2(960, 540),
		                           Vector2(1, 1),
		                           0.0f);

	mCurtain = new CurtainSprite(Vector2(960, 540),
		                         Vector2(1, 1),
		                         1.0f);

	mOperationExplanation = new HitPointBackGround(Vector2(1800, 540), Vector2(0.7, 0.7), 1.0, ("Assets/UI/OperationExplanation.png"));
}

GameSceneUI::~GameSceneUI()
{
}

void GameSceneUI::Update(float deltaTime)
{
	// フェードイン
	if (mIsFadeIn)
	{
		// フェードイン処理
		mCurtain->FadeIn(deltaTime);

		// フェードインが終わっていたらやめる
		if (IsFinishFadeIn())
		{
			mIsFadeIn = false;
		}
	}

	// プレイヤーのHPが0以下ならゲームオーバーを表示
	if(mPlayerHPGetter->GetHP() <= 0)
	{
		AUDIO->StopMusic("Assets/Sound/Battle.wav");
		mGameOver->Update(deltaTime);
		if (AUDIO->IsPlayingMusic("Assets/Sound/GameOver.wav"))
		{
			AUDIO->PlayMusic("Assets/Sound/GameOver.wav");
		}
		mIsFadeOut = true;
	}

	// 敵のHPが0以下ならゲームクリアを表示
	if (mEnemyHPGetter->GetHP() <= 0)
	{
		AUDIO->StopMusic("Assets/Sound/Battle.wav");
		mGameClear->Update(deltaTime);
		if (AUDIO->IsPlayingMusic("Assets/Sound/GameClear.wav"))
		{
			AUDIO->PlayMusic("Assets/Sound/GameClear.wav");
		}
		mIsFadeOut = true;
	}

	if (mIsFadeOut)
	{
		mCountTime += deltaTime;

		// 遅延時間が経過していたらフェードアウトを行う
		if (mCountTime > mDelayTime)
		{
			mCurtain->FadeOut(deltaTime);
		}
	}
}

void GameSceneUI::Draw()
{
	mPlayerHitPointBackGround->Draw();
	mPlayerHitPointGauge->Draw();
	mPlayerGaugeFrame->Draw();
	mEnemyHitPointBackGround->Draw();
	mEnemyHitPointGauge->Draw();
	mEnemyGaugeFrame->Draw();
	mOperationExplanation->Draw();
	mGameClear->Draw();
	mGameOver->Draw();
	mCurtain->Draw();
}

bool GameSceneUI::IsFinishFadeIn()
{
	if (mCurtain->GetAlpha() <= 0.0f)
	{
		return true;
	}

	return false;
}

bool GameSceneUI::IsFinishFadeOut()
{
	if (mCurtain->GetAlpha() >= 1.0f)
	{
		return true;
	}

	return false;
}
