#pragma once
#include "UIScreen.h"

class CurtainSprite : public UIScreen
{
public:
	CurtainSprite(const Vector2& pos, const Vector2& scale, const float& alpha);

	~CurtainSprite();

	void Update(float deltaTime);

	void Draw();

	void FadeIn(float deltaTime);

	void FadeOut(float deltaTime);

// �Q�b�^�[ //
	const float GetAlpha() { return mAlpha; }

private:

	// �t�F�[�h�C���̃X�s�[�h
	float mFadeInSpeed;

	// �ʏ�̃t�F�[�h�C���̃X�s�[�h
	const float mNormalFadeSpeed;

	// ���Z���邩
	bool IsAdition;
};