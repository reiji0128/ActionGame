#include "FireBallObject.h"
#include "GraphicResourceManager.h"
#include "SkeletalMeshComponent.h"
#include "AttachMeshComponent.h"
#include "EffectComponent.h"
#include "Mesh.h"
#include "BoxCollider.h"
#include "PointLightComponent.h"


FireBallObject::FireBallObject(const Vector3& pos, const Vector3& dir, const float& speed, Tag objectTag)
	:GameObject(objectTag)
	,mOffset(Vector3(0,100,0))
	,mBoxCollider(nullptr)
	,mPointLightComp(nullptr)
{
	mPosition  = pos;
	mPosition += mOffset;
	mDirection = dir;
	mSpeed     = speed;
	SetScale(0.5);

	// 当たり判定エリア作成
	AABB box;
	box.mMin = Vector3(-150, -150, -150);
	box.mMax = Vector3(150, 150, 150);
	box.mIsRotatable = false;
	BoxCollider* bc = new BoxCollider(this);
	bc->SetObjectBox(box);

	// エフェクト生成
	EffectComponent * ec = new EffectComponent(this, true, true);
	ec->LoadEffect(u"Assets/Effect/bullet.efk");

	mPointLightComp = new PointLightComponent(this, Vector3(0, 0, -100), Vector3(1.0f, 0.55f, 0.0f),1.0f);
}

FireBallObject::~FireBallObject()
{
}

void FireBallObject::UpdateGameObject(float deltaTime)
{
	mPosition += mDirection * mSpeed * deltaTime;
}

void FireBallObject::OnCollisionEnter(ColliderComponent* ownCollider, ColliderComponent* otherCollider)
{
	Tag colliderTag = otherCollider->GetTag();

	// 衝突情報
	CollisionInfo info;

	// プレイヤーかバッググラウンドに当たったら死亡状態に変更
	if (colliderTag == Tag::Player || colliderTag == Tag::BackGround)
	{
		mState = State::EDead;
	}
}
