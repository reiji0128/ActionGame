#pragma once

// �O���錾
class Health;
class HitPointBackGround;
class HitPointGauge;
class HitPointGaugeFrame;

class PlayerHitPointUI
{
public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="playerHealth">�v���C���[�̗̑�</param>
	PlayerHitPointUI(Health* playerHealth);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~PlayerHitPointUI();

private:

	/// <summary>
	/// �̗͂̕ύX���������Ƃ��ɌĂ΂��֐�
	/// </summary>
	void OnHealthChanged();

	// �v���C���[�̗̑�
	Health* mPlayerHealth;

	// �̗̓Q�[�W�̔w�i
	HitPointBackGround* mBackGround;

	// �̗̓Q�[�W
	HitPointGauge* mGauge;

	// �̗̓Q�[�W�̃t���[��
	HitPointGaugeFrame* mFrame;
};
