#pragma once
#include "UIScreen.h"

class HitPointGaugeFrame : public UIScreen
{
public:
	HitPointGaugeFrame(const Vector2& offset, const Vector2& scale,const float& alpha,const char* texFilePass);

	~HitPointGaugeFrame();

	void Draw();
};