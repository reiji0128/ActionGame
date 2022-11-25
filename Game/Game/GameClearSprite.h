#pragma once
#include "UIScreen.h"

class GameClearSprite : public UIScreen
{
public:
	GameClearSprite(const Vector2& pos, const Vector2& scale, const float& alpha);

	~GameClearSprite();

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