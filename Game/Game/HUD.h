#pragma once
#include "Math.h"

class Texture;

class HUD
{
public:
	HUD();

	~HUD();

protected:
	Vector2 mOffsetPos;

	Vector2 mScale;

	Texture* mTexture;
};
