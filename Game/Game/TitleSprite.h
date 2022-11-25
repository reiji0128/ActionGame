#pragma once
#include "UIScreen.h"

class TitleSprite : public UIScreen
{
public:
	TitleSprite(const Vector2& pos, const Vector2& scale, const float& alpha, const char* texFilePass);

	~TitleSprite();

	void Draw();
};