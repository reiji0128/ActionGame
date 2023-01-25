#include "BossHitPointUI.h"
#include <functional>
#include "Health.h"
#include "HitPointBackGround.h"
#include "HitPointGauge.h"
#include "HitPointGaugeFrame.h"

BossHitPointUI::BossHitPointUI(Health* bossHealth)
	:mBossHealth(bossHealth)
{

	mBackGround = new HitPointBackGround(Vector2(944.0f, 900.0f),          // 相対座標
		                                 Vector2(0.82f, 0.9f),             // 拡大率
		                                 0.5f,                             // 透過度
		                                 "Assets/UI/Enemy/EnemyBack.png"); // テクスチャのファイルパス

	mGauge = new HitPointGauge(Vector2(500.0f, 900.0f),                    // 相対座標
		                       Vector2(1.6f, 0.025f),                      // 拡大率
		                       1.0,                                        // 透過度
		                       "Assets/UI/Enemy/Health.png",               // テクスチャのファイルパス
		                       mBossHealth->GetMaxHealth());               // 最大体力
																		   
	mFrame = new HitPointGaugeFrame(Vector2(950.0f, 900.0f),               // 相対座標
		                            Vector2(0.81f, 2.0f),                  // 拡大率
		                            1.0f,                                  // 透過度
		                            "Assets/UI/Enemy/EnemyFrame.png");     // テクスチャのファイルパス

	// 体力の変更があったときに呼ばれる関数のポインタを体力クラスに登録
	std::function<void()> onHealthChanged = std::bind(&BossHitPointUI::OnHealthChanged, this);
	mBossHealth->RegisterFunc(onHealthChanged);
}

BossHitPointUI::~BossHitPointUI()
{
}

void BossHitPointUI::OnHealthChanged()
{
	// 現在の体力をセット
	float nowHP = mBossHealth->GetCurrentHealth();
	mGauge->SetNowHP(nowHP);
}
