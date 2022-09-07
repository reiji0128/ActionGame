﻿#include "Component.h"
#include "GameObject.h"

// 連番用の管理IDを初期化
int Component::mGlobalID = 0;

/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="owner">ゲームオブジェクトのポインタ</param>
	/// <param name="updateOrder">更新順序(更新順序が低いほど、コンポーネントの更新が早い)</param>
Component::Component(GameObject* owner, int updateOrder)
	: mUpdateOrder(updateOrder)
	, mID()
	, mOwner(owner)
	, mState(EActive)
{
	// ゲームオブジェクトにこのコンポーネントを追加
	mOwner->AddComponent(this);

	mID = mGlobalID;
	mGlobalID++;
}

/// <summary>
/// デストラクタ
/// </summary>
Component::~Component()
{
	// コンポーネントの削除
	mOwner->RemoveComponent(this);
}

/// <summary>
/// コンポーネントの更新処理
/// </summary>
/// <param name="deltaTime"></param>
void Component::Update(float deltaTime)
{
}
