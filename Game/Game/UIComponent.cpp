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
	// �X�N���[���T�C�Y�̃Z�b�g
	mScreenWidth  = RENDERER->GetScreenWidth();
	mScreenHeight = RENDERER->GetScreenHeight();

	// �R���|�[�l���g�̒ǉ�
	mOwner->AddComponent(this);
}

UIComponent::~UIComponent()
{
	// �R���|�[�l���g�̍폜
	mOwner->RemoveComponent(this);
}
