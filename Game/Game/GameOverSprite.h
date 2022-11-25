#pragma once
#include "UIScreen.h"

class GameOverSprite : public UIScreen
{
public:
	GameOverSprite(const Vector2& pos, const Vector2& scale, const float& alpha);

	~GameOverSprite();

	void Update(float deltaTime);

	void Draw();

	void FadeIn(float deltaTime);

private:

	// �t�F�[�h�C���̃X�s�[�h
	float mFadeInSpeed;

	// �ʏ�̃t�F�[�h�C���̃X�s�[�h
	const float mNormalFadeSpeed;

	// ���Z���邩
	bool IsAdition;
};