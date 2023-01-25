#pragma once
#include "UIComponent.h"

class GaugeComponent : public UIComponent
{
public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="owner">�R���|�[�l���g�̏��L��</param>
	/// <param name="drawOrder">�`�揇��</param>
	GaugeComponent(UIScreen* owner, Texture* texture, float maxValue, int drawOrderfloat = 100);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~GaugeComponent();

	/// <summary>
	/// �`�揈��
	/// </summary>
	/// <param name="shader">�g�p����V�F�[�_�[</param>
	void Draw(Shader* shader) override;

	/// <summary>
	/// ���݂̒l�̃Z�b�g
	/// </summary>
	/// <param name="nowValue">���݂̒l</param>
	void SetNowValue(float nowValue) { mNowValue = nowValue; }

	/// <summary>
	/// �Q�[�W�̃X�P�[���v�Z
	/// </summary>
	void CalcGaugeScale(const float& nowValue);

private:


	// ���݂̒l
	float mNowValue;

	// �ő�̒l
	float mMaxValue;

	// �ŏI�I�ȃX�P�[��
	float mResultScale;
};