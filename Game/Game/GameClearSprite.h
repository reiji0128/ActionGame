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

	// フェードインのスピード
	float mFadeInSpeed;

	// 通常のフェードインのスピード
	const float mNormalFadeSpeed;

	// 加算するか
	bool IsAdition;

};