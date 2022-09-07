#include "SkyBoxObject.h"
#include "CubeMapComponent.h"
#include "Game.h"
#include "Renderer.h"

SkyBoxObject::SkyBoxObject()
	:GameObject(Tag::SkyBox)
{
	// キューブマップコンポーネントを生成
	mCubeMapComp = new CubeMapComponent(this);

	// レンダラーにキューブマップコンポーネントのポインタを渡す
	RENDERER->SetActiveSkyBox(mCubeMapComp);

	// 使用するテクスチャを生成
	mCubeMapComp->CreateTexture("Assets/Skybox/");
}

SkyBoxObject::~SkyBoxObject()
{
}
