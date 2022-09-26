#pragma once
#include "HUD.h"

class PlayerHealthBar : public HUD
{
public:
	PlayerHealthBar();

	~PlayerHealthBar();

	void Draw() override;
};