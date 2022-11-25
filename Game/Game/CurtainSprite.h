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

// ゲッター //
	const float GetAlpha() { return mAlpha; }

private:

	// フェードインのスピード
	float mFadeInSpeed;

	// 通常のフェードインのスピード
	const float mNormalFadeSpeed;

	// 加算するか
	bool IsAdition;
};