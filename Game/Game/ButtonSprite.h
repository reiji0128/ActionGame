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

	// フェードインのスピード
	float mFadeInSpeed;

	// 通常のフェードインのスピード
	const float mNormalFadeSpeed;

	// 決定時のフェードインのスピード
	const float mDecisionFadeSpeed;

	// 加算するか
	bool IsAdition;
};