#pragma once
#include "GameObject.h"

// �O���錾
class CubeMapComponent;

class SkyBoxObject : public GameObject
{
public:
	SkyBoxObject();

	~SkyBoxObject();

private:
	CubeMapComponent* mCubeMapComp;
};