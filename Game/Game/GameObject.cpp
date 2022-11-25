#include "GameObject.h"
#include "Game.h"
#include "Component.h"

#include <algorithm>
#include <typeinfo>

// 管理ID用の静的カウンタの初期化
int GameObject::mGlobalObjectNo = 0;

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="objectTag">オブジェクトのタグ</param>
GameObject::GameObject(Tag objectTag)
	:mTag(objectTag)
	,mState(EActive)
	,mPosition(Vector3::Zero)
	,mDirection(Vector3::UnitX)
	,mRotation(Quaternion::Identity)
	,mScale(1.0f)
	,mRecomputeWorldTransform(true)
	,mID(mGlobalObjectNo)
	,mSpeed(0.0f)
{
	//ゲームシステム本体にゲームオブジェクトを追加
	GAMEINSTANCE.AddGameObject(this);
	mGlobalObjectNo++;
}

/// <summary>
/// デストラクタ
/// </summary>
GameObject::~GameObject()
{
	// ゲームオブジェクトが持っているコンポーネントの削除
	while (!mComponents.empty())
	{
		delete mComponents.back();
	}
	// ゲームシステム本体にこのゲームオブジェクトの削除を依頼
	GAMEINSTANCE.RemoveGameObject(this);
}

/// <summary>
/// 更新処理
/// </summary>
/// <param name="deltaTime">1フレームの経過時間</param>
void GameObject::Update(float deltaTime)
{
	// ゲームオブジェクトが生きているときのみ処理
	// ワールド行列の再計算、コンポーネントとゲームオブジェクトの更新処理を行う
	if (mState == EActive)
	{
		ComputeWorldTransform();

		UpdateComponents(deltaTime);
		UpdateGameObject(deltaTime);

		ComputeWorldTransform();
	}
}

/// <summary>
/// コンポーネントの更新処理
/// </summary>
/// <param name="deltaTime">1フレームの経過時間</param>
void GameObject::UpdateComponents(float deltaTime)
{
	for (auto comp : mComponents)
	{
		comp->Update(deltaTime);
	}

	// 死んでいるコンポーネントを一時保管
	for (auto comp : mComponents)
	{
		if (comp->GetState() == Component::EDelete)
		{
			mDeleteComponents.emplace_back(comp);
		}
	}

	// 消去コンポーネントをdelete mComponentsからもDeleteされる
	for (auto comp : mDeleteComponents)
	{
		delete comp;
	}
	mDeleteComponents.clear();
}

/// <summary>
/// ゲームオブジェクト専用の更新処理
/// </summary>
/// <param name="deltaTime">1フレームの経過時間</param>
void GameObject::UpdateGameObject(float deltaTime)
{
}

/// <summary>
/// 入力処理
/// </summary>
void GameObject::ProcessInput()
{
	if (mState == EActive)
	{
		// 入力処理を受け取るコンポーネントを優先して実行
		for (auto comp : mComponents)
		{
			comp->ProcessInput();
		}
	}
}

/// <summary>
/// 衝突処理
/// </summary>
/// <param name="ownCollider">ゲームオブジェクトの所有するColliderComponentのポインタ</param>
/// <param name="otherCollider">衝突相手のColliderComponentのポインタ</param>
void GameObject::OnCollisionEnter(ColliderComponent* ownCollider, ColliderComponent* otherCollider)
{
}

/// <summary>
/// 前進ベクトルから回転を計算
/// </summary>
void GameObject::RotateToNewForward()
{
	// X軸ベクトル(1,0,0)とmDirectionの間の角度を求める
	float dot = Vector3::Dot(Vector3::UnitX, mDirection);
	float angle = Math::Acos(dot);
	// 下向きだった場合
	if (dot > 0.9999f)
	{
		SetRotation(Quaternion::Identity);
	}
	// 上向きだった場合
	else if (dot < -0.9999f)
	{
		SetRotation(Quaternion(Vector3::UnitZ, Math::Pi));
	}
	else
	{
		// 軸ベクトルとmDirectionとの外積から回転軸をもとめ、回転させる
		Vector3 axis = Vector3::Cross(Vector3::UnitX, mDirection);
		axis.Normalize();
		SetRotation(Quaternion(axis, angle));
	}
}

/// <summary>
/// 前方ベクトルから回転を計算
/// </summary>
/// <param name="forward">前方ベクトル</param>
void GameObject::RotateToNewForward(const Vector3& forward)
{
	// X軸ベクトル(1,0,0)とmDirectionの間の角度を求める
	float dot = Vector3::Dot(Vector3::UnitX, forward);
	float angle = Math::Acos(dot);

	// 下向きだった場合
	if (dot > 0.9999f)
	{
		SetRotation(Quaternion::Identity);
	}
	// 上向きだった場合
	else if (dot < -0.9999f)
	{
		SetRotation(Quaternion(Vector3::UnitZ, Math::Pi));
	}
	else
	{
		// 軸ベクトルとmDirectionとの外積から回転軸をもとめ、回転させる
		Vector3 axis = Vector3::Cross(Vector3::UnitX, forward);
		axis.Normalize();
		SetRotation(Quaternion(axis, angle));
	}
}

/// <summary>
/// ワールド変換行列の計算
/// </summary>
void GameObject::ComputeWorldTransform()
{
	//ワールド変換の再計算が必要なら実行
	if (mRecomputeWorldTransform)
	{
		mRecomputeWorldTransform = false;

		//前進ベクトルから回転を計算
		RotateToNewForward();

		// スケーリング→回転→平行移動となるように変換行列を作成
		mWorldTransform = Matrix4::CreateScale(mScale);
		mWorldTransform *= Matrix4::CreateFromQuaternion(mRotation);
		mWorldTransform *= Matrix4::CreateTranslation(mPosition);

		// アクターが持っている全コンポーネントの変換を促す
		for (auto comp : mComponents)
		{
			comp->OnUpdateWorldTransform();
		}
	}
}

/// <summary>
/// コンポーネントの追加
/// </summary>
/// <param name="component">追加するComponentのポインタ</param>
void GameObject::AddComponent(Component* addComponent)
{
	// コンポーネントをソートして追加
	// 自分のオーダー番号よりも大きい挿入点を見つける
	int myOrder = addComponent->GetUpdateOrder();
	auto iter = mComponents.begin();
	for (;
		iter != mComponents.end();
		++iter)
	{
		if (myOrder < (*iter)->GetUpdateOrder())
		{
			break;
		}
	}
	// 要素を見つけたポイントの手前に挿入する
	mComponents.insert(iter, addComponent);
}

/// <summary>
/// コンポーネントの削除 
/// </summary>
/// <param name="component">削除するComponentのポインタ</param>
void GameObject::RemoveComponent(Component* removeComponent)
{
	auto iter = std::find(mComponents.begin(), mComponents.end(), removeComponent);
	if (iter != mComponents.end())
	{
		mComponents.erase(iter);
	}
}