#pragma once
#include "UIScreen.h"

class HitPointBackGround : public UIScreen
{
public:
	HitPointBackGround(const Vector2& offset, const Vector2& scale, const float& alpha, const char* texFilePass);

	~HitPointBackGround();

	void Draw();
};