#include "UIScreen.h"
#include "Game.h"
#include "Renderer.h"
#include "GraphicResourceManager.h"
#include "Shader.h"
#include "Texture.h"

UIScreen::UIScreen(const Vector2& offset, const Vector2& scale, const float& alpha)
	:mOffset(offset)
	,mScale(scale)
	,mAlpha(alpha)
	,mTexture(nullptr)
{
	mScreenWidth  = RENDERER->GetScreenWidth();
	mScreenHeight = RENDERER->GetScreenHeight();
}

UIScreen::~UIScreen()
{
}

