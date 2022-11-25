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
	// �v���C���[��HP�Q�[�W�֘A //
	mPlayerHitPointBackGround = new HitPointBackGround(Vector2(390, 115),                  // ���΍��W
		                                               Vector2(0.82, 1.0),                 // �g�嗦
		                                               0.5,                                // ���ߓx
		                                               "Assets/UI/Player/PlayerBack.png"); // �e�N�X�`���̃t�@�C���p�X

	mPlayerHitPointGauge = new HitPointGauge(Vector2(100, 100),                            // ���΍��W
		                                     Vector2(1, 0.03f),                            // �g�嗦
		                                     1.0,                                          // ���ߓx
		                                     "Assets/UI/Player/Health.png",                // �e�N�X�`���̃t�@�C���p�X
		                                     playerHitPointGetter);                        // �C���^�[�t�F�C�X

	mPlayerGaugeFrame = new HitPointGaugeFrame(Vector2(392, 130),                          // ���΍��W
		                                       Vector2(0.82, 2.0), 					       // �g�嗦
		                                       1.0,									       // ���ߓx
		                                       "Assets/UI/Player/PlayerFrame.png");	       // �e�N�X�`���̃t�@�C���p�X

// �G�l�~�[��HP�Q�[�W�֘A //
    mEnemyHitPointBackGround = new HitPointBackGround(Vector2(944, 900),                   // ���΍��W
		                                              Vector2(0.82, 0.9),                  // �g�嗦
		                                              0.5,                                 // ���ߓx
		                                              "Assets/UI/Enemy/EnemyBack.png");    // �e�N�X�`���̃t�@�C���p�X

	mEnemyHitPointGauge = new HitPointGauge(Vector2(500, 900),                             // ���΍��W
		                                    Vector2(1.6, 0.025f),                          // �g�嗦
		                                    1.0,                                           // ���ߓx
		                                    "Assets/UI/Enemy/Health.png",                  // �e�N�X�`���̃t�@�C���p�X
		                                    enemyHitPointGetter);                          // �C���^�[�t�F�C�X

	mEnemyGaugeFrame = new HitPointGaugeFrame(Vector2(950, 900),                           // ���΍��W
		                                      Vector2(0.81, 2.0),                          // �g�嗦
		                                      1.0,                                         // ���ߓx
		                                      "Assets/UI/Enemy/EnemyFrame.png");           // �e�N�X�`���̃t�@�C���p�X

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
	// �t�F�[�h�C��
	if (mIsFadeIn)
	{
		// �t�F�[�h�C������
		mCurtain->FadeIn(deltaTime);

		// �t�F�[�h�C�����I����Ă������߂�
		if (IsFinishFadeIn())
		{
			mIsFadeIn = false;
		}
	}

	// �v���C���[��HP��0�ȉ��Ȃ�Q�[���I�[�o�[��\��
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

	// �G��HP��0�ȉ��Ȃ�Q�[���N���A��\��
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

		// �x�����Ԃ��o�߂��Ă�����t�F�[�h�A�E�g���s��
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
