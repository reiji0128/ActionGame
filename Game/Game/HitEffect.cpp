#include "HitEffect.h"
#include "EffectComponent.h"

HitEffect::HitEffect(GameObject* owner,const Vector3& offset)
	:GameObject(Tag::Effect)
	,mOffset(offset)
	,mOwner(owner)
	,mEffectComp(nullptr)
{

	mEffectComp = new EffectComponent(this, true, false);
	mEffectComp->LoadEffect(u"Assets/Effect/HitEffect.efk");
}

HitEffect::~HitEffect()
{
}

void HitEffect::UpdateGameObject(float deltaTime)
{
}

void HitEffect::PlayEffect()
{
	mPosition = mOwner->GetPosition();
	mPosition += mOffset;
	mEffectComp->PlayEffect(1.0f);
}
