#pragma once

// �O���錾
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

	// �J�E���g�^�C��
	float mCountTime;

	// �x������
	const float mDelayTime;

	// �t�F�[�h�C�����邩
	bool mIsFadeIn;

	// �t�F�[�h�A�E�g���邩
	bool mIsFadeOut;

	IHitPointGetter* mPlayerHPGetter;

	IHitPointGetter* mEnemyHPGetter;

	// �v���C���[��HP�Q�[�W�֘A
	HitPointBackGround* mPlayerHitPointBackGround;
	HitPointGauge* mPlayerHitPointGauge;
	HitPointGaugeFrame* mPlayerGaugeFrame;

	// �G�l�~�[��HP�Q�[�W�֘A
	HitPointBackGround* mEnemyHitPointBackGround;
	HitPointGauge* mEnemyHitPointGauge;
	HitPointGaugeFrame* mEnemyGaugeFrame;

	// �Q�[���N���A
	GameClearSprite* mGameClear;

	// �Q�[���I�[�o�[
	GameOverSprite* mGameOver;

	// �������
	HitPointBackGround* mOperationExplanation;

	// �t�F�[�h�C���E�t�F�[�h�A�E�g�̖�
	CurtainSprite* mCurtain;
};