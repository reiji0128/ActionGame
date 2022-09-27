#pragma once
#include "Math.h"

class Texture;

class HUD
{
public:
	HUD() {};

	~HUD() {};

	virtual void Draw() = 0;

protected:
	Vector2 mOffsetPos;

	Vector2 mScale;

	float mAlpha;

	Texture* mTexture;
};
