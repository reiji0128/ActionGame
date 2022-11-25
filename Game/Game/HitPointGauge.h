#pragma once
#include "UIScreen.h"

class IHitPointGetter;

class HitPointGauge : public UIScreen
{
public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="offset">���΍��W</param>
	/// <param name="scale">�g�嗦</param>
	/// <param name="alpha">���ߓx</param>
	/// <param name="textureFilePass">�e�N�X�`���̃t�@�C���p�X</param>
	HitPointGauge(const Vector2& offset, const Vector2& scale, float alpha, const char* textureFilePass, IHitPointGetter* playerParameterGetter);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~HitPointGauge();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw() override;

	/// <summary>
	/// �̗̓Q�[�W�̃X�P�[���v�Z
	/// </summary>
	void CalcGaugeScale();

private:
	// ���݂̗̑�
	float mNowHP;
	
	// �ő�̗�
	float mMaxHP;

	float resultScale;

	// IPlayerParameterGetter�N���X�̃|�C���^
	IHitPointGetter* mHitPointGetter;
};