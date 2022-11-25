#pragma once
#include "UIScreen.h"

class ButtonSprite : public UIScreen
{
public:
	ButtonSprite(const Vector2& offset, const Vector2& scale, const float& alpha, const char* texFilePass);

	~ButtonSprite();

	void Update(float deltaTime);

	void Draw();

	void FadeIn(float deltaTime);

	void FadeOut(float deltaTime);

private:

	// �t�F�[�h�C���̃X�s�[�h
	float mFadeInSpeed;

	// �ʏ�̃t�F�[�h�C���̃X�s�[�h
	const float mNormalFadeSpeed;

	// ���莞�̃t�F�[�h�C���̃X�s�[�h
	const float mDecisionFadeSpeed;

	// ���Z���邩
	bool IsAdition;
};