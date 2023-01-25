#include "UIComponent.h"
#include "UIScreen.h"
#include "Game.h"
#include "Renderer.h"

UIComponent::UIComponent(UIScreen* owner, UITag tag,int drawOrder)
	:mOwner(owner)
	,mDrawOrder(drawOrder)
	,mUITag(tag)
	,mOffset(Vector2::Zero)
	,mScale(Vector2::Zero)
	,mAlpha(0.0f)
	,mTexture(nullptr)
	,mVisible(true)
{
	// スクリーンサイズのセット
	mScreenWidth  = RENDERER->GetScreenWidth();
	mScreenHeight = RENDERER->GetScreenHeight();

	// コンポーネントの追加
	mOwner->AddComponent(this);
}

UIComponent::~UIComponent()
{
	// コンポーネントの削除
	mOwner->RemoveComponent(this);
}
