#include "FireBreathObject.h"
#include "EffectComponent.h"
#include "BoxCollider.h"
#include "PointLightComponent.h"
#include "AttackObj.h"

FireBreathObject::FireBreathObject(const Vector3& offset, const Vector3& dir, Tag objectTag)
	:GameObject(objectTag)
{
	mPosition = offset;
	mDirection = dir;
	SetScale(1.0);

	Vector3 collisionPos = mPosition;
	collisionPos.z -= 50;

	// エフェクト生成
	EffectComponent* ec = new EffectComponent(this, true, true);
	ec->LoadEffect(u"Assets/Effect/Breath.efk");

	//mPointLightComp = new PointLightComponent(this, Vector3(0, 0, 50), Vector3(1.0f, 0.55f, 0.0f),5.0f);

	// 当たり判定の生成
	new AttackObj(Tag::EnemyFireBreath, collisionPos, Vector3( 300, 300, 0), mDirection, 180.0f, 14.0f);
	new AttackObj(Tag::EnemyFireBreath, collisionPos, Vector3( 480, 480, 0), mDirection, 180.0f, 14.0f);
	new AttackObj(Tag::EnemyFireBreath, collisionPos, Vector3( 660, 660, 0), mDirection, 180.0f, 14.0f);
	new AttackObj(Tag::EnemyFireBreath, collisionPos, Vector3( 840, 840, 0), mDirection, 180.0f, 14.0f);
	new AttackObj(Tag::EnemyFireBreath, collisionPos, Vector3(1020, 1020, 0), mDirection, 180.0f, 14.0f);
	new AttackObj(Tag::EnemyFireBreath, collisionPos, Vector3(1200, 1200, 0), mDirection, 180.0f, 14.0f);
	//new AttackObj(Tag::EnemyFireBreath, collisionPos, Vector3(1380, 1380, 0), mDirection, 180.0f, 14.0f);
}

FireBreathObject::~FireBreathObject()
{
}

void FireBreathObject::UpdateGameObject(float deltaTime)
{
	/*mMouthPos.x -= 500;
	mMouthPos.y -= 100;
	mPosition = mMouthPos;*/
}

void FireBreathObject::OnCollisionEnter(ColliderComponent* ownCollider, ColliderComponent* otherCollider)
{
}
