#pragma once
#include "GameObject.h"

// ‘O•ûéŒ¾
class CubeMapComponent;

class SkyBoxObject : public GameObject
{
public:
	SkyBoxObject();

	~SkyBoxObject();

private:
	CubeMapComponent* mCubeMapComp;
};