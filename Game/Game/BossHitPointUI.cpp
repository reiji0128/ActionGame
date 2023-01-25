#include "BossHitPointUI.h"
#include <functional>
#include "Health.h"
#include "HitPointBackGround.h"
#include "HitPointGauge.h"
#include "HitPointGaugeFrame.h"

BossHitPointUI::BossHitPointUI(Health* bossHealth)
	:mBossHealth(bossHealth)
{

	mBackGround = new HitPointBackGround(Vector2(944.0f, 900.0f),          // ���΍��W
		                                 Vector2(0.82f, 0.9f),             // �g�嗦
		                                 0.5f,                             // ���ߓx
		                                 "Assets/UI/Enemy/EnemyBack.png"); // �e�N�X�`���̃t�@�C���p�X

	mGauge = new HitPointGauge(Vector2(500.0f, 900.0f),                    // ���΍��W
		                       Vector2(1.6f, 0.025f),                      // �g�嗦
		                       1.0,                                        // ���ߓx
		                       "Assets/UI/Enemy/Health.png",               // �e�N�X�`���̃t�@�C���p�X
		                       mBossHealth->GetMaxHealth());               // �ő�̗�
																		   
	mFrame = new HitPointGaugeFrame(Vector2(950.0f, 900.0f),               // ���΍��W
		                            Vector2(0.81f, 2.0f),                  // �g�嗦
		                            1.0f,                                  // ���ߓx
		                            "Assets/UI/Enemy/EnemyFrame.png");     // �e�N�X�`���̃t�@�C���p�X

	// �̗͂̕ύX���������Ƃ��ɌĂ΂��֐��̃|�C���^��̗̓N���X�ɓo�^
	std::function<void()> onHealthChanged = std::bind(&BossHitPointUI::OnHealthChanged, this);
	mBossHealth->RegisterFunc(onHealthChanged);
}

BossHitPointUI::~BossHitPointUI()
{
}

void BossHitPointUI::OnHealthChanged()
{
	// ���݂̗̑͂��Z�b�g
	float nowHP = mBossHealth->GetCurrentHealth();
	mGauge->SetNowHP(nowHP);
}
