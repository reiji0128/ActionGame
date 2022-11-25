#pragma once
#include "GameObject.h"
#include "ShaderTag.h"

class PointLight : GameObject
{
public:

	PointLight(const Vector3 &pos,const Vector3& dir);

	~PointLight();

private:
	ShaderTag mShaderTag;
};