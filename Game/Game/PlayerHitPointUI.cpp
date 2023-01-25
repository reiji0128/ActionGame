#include "PlayerHitPointUI.h"
#include <functional>
#include "Health.h"
#include "HitPointBackGround.h"
#include "HitPointGauge.h"
#include "HitPointGaugeFrame.h"

PlayerHitPointUI::PlayerHitPointUI(Health* playerHealth)
	:mPlayerHealth(playerHealth)
{
	// 体力ゲージの背景の生成
	mBackGround = new HitPointBackGround(Vector2(390.0f, 115.0f),            // 相対座標
		                                 Vector2(0.82f, 1.0f),               // 拡大率
		                                 0.5f,                               // 透過度
		                                 "Assets/UI/Player/PlayerBack.png"); // テクスチャのファイルパス

	// 体力ゲージの生成
	mGauge = new HitPointGauge(Vector2(100.0f, 100.0f),                      // 相対座標
		                       Vector2(1.0f, 0.03f),                         // 拡大率
		                       1.0f,                                         // 透過度
		                       "Assets/UI/Player/Health.png",                // テクスチャのファイルパス
		                       mPlayerHealth->GetMaxHealth());               // 最大体力

	// 体力ゲージのフレームの生成
	mFrame = new HitPointGaugeFrame(Vector2(392.0f, 130.0f),                 // 相対座標
	                                Vector2(0.82f, 2.0f), 					 // 拡大率
	                                1.0,									 // 透過度
	                                "Assets/UI/Player/PlayerFrame.png");	 // テクスチャのファイルパス

	// 体力の変更があったときに呼ばれる関数のポインタを体力クラスに登録
	std::function<void()> onHealthChanged = std::bind(&PlayerHitPointUI::OnHealthChanged,this);
	mPlayerHealth->RegisterFunc(onHealthChanged);
}

PlayerHitPointUI::~PlayerHitPointUI()
{
	delete mBackGround;
	delete mGauge;
	delete mFrame;
}

void PlayerHitPointUI::OnHealthChanged()
{
	// 現在の体力をセット
	float nowHP = mPlayerHealth->GetCurrentHealth();
	mGauge->SetNowHP(nowHP);
}
