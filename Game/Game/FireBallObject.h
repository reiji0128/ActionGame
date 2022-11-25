#pragma once
#include "GameObject.h"

// 前方宣言
class PointLightComponent;
class BoxCollider;


class FireBallObject : public GameObject 
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="offset">座標</param>
	/// <param name="objectTag">オブジェクトのタグ</param>
	FireBallObject(const Vector3& pos,const Vector3& dir,const float& speed,Tag objectTag);

	
	~FireBallObject();

	/// <summary>
	/// ゲームオブジェクトの更新処理
	/// </summary>
	/// <param name="deltaTime"></param>
	void UpdateGameObject(float deltaTime) override;

	/// 衝突処理
	/// </summary>
	/// <param name="ownCollider">自分のコライダー</param>
	/// <param name="otherCollider">相手のコライダー</param>
	void OnCollisionEnter(ColliderComponent* ownCollider, ColliderComponent* otherCollider) override;

private:

	// オフセット
	const Vector3 mOffset;

	BoxCollider* mBoxCollider;

	PointLightComponent* mPointLightComp;
};