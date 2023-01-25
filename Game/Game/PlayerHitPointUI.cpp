#include "PlayerHitPointUI.h"
#include <functional>
#include "Health.h"
#include "HitPointBackGround.h"
#include "HitPointGauge.h"
#include "HitPointGaugeFrame.h"

PlayerHitPointUI::PlayerHitPointUI(Health* playerHealth)
	:mPlayerHealth(playerHealth)
{
	// �̗̓Q�[�W�̔w�i�̐���
	mBackGround = new HitPointBackGround(Vector2(390.0f, 115.0f),            // ���΍��W
		                                 Vector2(0.82f, 1.0f),               // �g�嗦
		                                 0.5f,                               // ���ߓx
		                                 "Assets/UI/Player/PlayerBack.png"); // �e�N�X�`���̃t�@�C���p�X

	// �̗̓Q�[�W�̐���
	mGauge = new HitPointGauge(Vector2(100.0f, 100.0f),                      // ���΍��W
		                       Vector2(1.0f, 0.03f),                         // �g�嗦
		                       1.0f,                                         // ���ߓx
		                       "Assets/UI/Player/Health.png",                // �e�N�X�`���̃t�@�C���p�X
		                       mPlayerHealth->GetMaxHealth());               // �ő�̗�

	// �̗̓Q�[�W�̃t���[���̐���
	mFrame = new HitPointGaugeFrame(Vector2(392.0f, 130.0f),                 // ���΍��W
	                                Vector2(0.82f, 2.0f), 					 // �g�嗦
	                                1.0,									 // ���ߓx
	                                "Assets/UI/Player/PlayerFrame.png");	 // �e�N�X�`���̃t�@�C���p�X

	// �̗͂̕ύX���������Ƃ��ɌĂ΂��֐��̃|�C���^��̗̓N���X�ɓo�^
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
	// ���݂̗̑͂��Z�b�g
	float nowHP = mPlayerHealth->GetCurrentHealth();
	mGauge->SetNowHP(nowHP);
}
