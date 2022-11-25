#pragma once

class IHitPointGetter
{
public:
	IHitPointGetter() {};

	virtual ~IHitPointGetter() {};

	virtual int GetHP() const = 0;

	virtual int GetMaxHP() const = 0;
};