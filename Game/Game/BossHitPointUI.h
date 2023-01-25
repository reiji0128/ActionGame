#pragma once

// �O���錾
class Health;
class HitPointBackGround;
class HitPointGauge;
class HitPointGaugeFrame;

class BossHitPointUI
{
public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="playerHealth">�{�X�̗̑�</param>
	BossHitPointUI(Health* bossHealth);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~BossHitPointUI();

private:

	/// <summary>
	/// �̗͂̕ύX���������Ƃ��ɌĂ΂��֐�
	/// </summary>
	void OnHealthChanged();

	// �{�X�̗̑�
	Health* mBossHealth;

	// �̗̓Q�[�W�̔w�i
	HitPointBackGround* mBackGround;

	// �̗̓Q�[�W
	HitPointGauge* mGauge;

	// �̗̓Q�[�W�̃t���[��
	HitPointGaugeFrame* mFrame;
};