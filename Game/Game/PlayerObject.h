#pragma once
#include "GameObject.h"
#include "IPlayerParameterGetter.h"
#include <vector>
#include <functional>
#include "SkeletalMeshComponent.h"
#include "Math.h"
#include "ShaderTag.h"


// プレイヤーの状態
enum class PlayerState
{
	STATE_IDLE,     // 待機
	STATE_RUN,      // 走っている
	STATE_FIRST_ATTACK,
	STATE_SECOND_ATTACK,
	STATE_DAMAGE,    
	STATE_DEATH,

	STATE_NUM       // 総アニメーション数
};

class PlayerObject :public GameObject, public IPlayerParameterGetter
{
public:

	PlayerObject(const Vector3& pos, const float& scale, const char* gpmeshFileName, const char* gpskelFileName);

	~PlayerObject();

	/// <summary>
	/// プレイヤーの更新処理
	/// </summary>
	/// <param name="deltaTime">1フレームの経過時間</param>
	void UpdateGameObject(float deltaTime)override;

	/// <summary>
	/// 衝突処理
	/// </summary>
	/// <param name="ownCollider"></param>
	/// <param name="otherCollider"></param>
	void OnCollisionEnter(class ColliderComponent* ownCollider, class ColliderComponent* otherCollider)override;

	void Damage(int damage);

// セッター //
	/// <summary>
	/// 接地フラグのセット
	/// </summary>
	/// <param name="IsOnGround">false : 接地していない
	///                          true  : 接地している
	/// </param>
	bool SetIsGround(bool IsOnGround) { mIsOnGround = IsOnGround; }

// ゲッター //

	int GetHP() const override { return mHP; }

	int GetMaxHP() const override { return mMaxHP; }

	/// <summary>
	/// 接地フラグのゲッター
	/// </summary>
	/// <returns>接地フラグ</returns>
	bool GetIsOnGround() { return mIsOnGround; }

	/// <summary>
	/// スケルタルメッシュの取得
	/// </summary>
	/// <returns>スケルタルメッシュコンポーネントのポインタ</returns>
	class SkeletalMeshComponent* GetSkeltalMeshComp() { return mSkeltalMeshComp; }
	
	/// <summary>
	/// アニメーションの取得
	/// </summary>
	/// <param name="state">プレイヤーの状態</param>
	/// <returns>アニメーション</returns>
	const Animation* GetAnim(PlayerState state) const { return mAnimations[static_cast<unsigned int>(state)]; }



private:
	// 体力
	int mHP;

	// 最大体力
	const int mMaxHP = 100;

	// 移動スピード
	Vector3 mVelocity;

	// ボーンのインデックス
	int mBoneIndex;

	// ボーンの行列
	Matrix4 mBoneMatrix;

	// プレイヤーの現在の状態
	PlayerState mNowState;

	// プレイヤーの次の状態
	PlayerState mNextState;

	// 接地しているか
	bool mIsOnGround;

	// ダメージを受けているか
	bool mIsDamage;

	// アニメーション可変長コンテナ
	std::vector<const class Animation*> mAnimations;

	// ステートクラスプール
	std::vector<class PlayerStateBase*> mStatePools;

	// スケルタルメッシュコンポーネントのポインタ
	class SkeletalMeshComponent* mSkeltalMeshComp;

	// ボックスコライダーのポインタ
	class BoxCollider* mBoxCollider;

	// ラインコライダーのポインタ
	class LineCollider* mLineCollider;

	// 適用するシェーダーのタグ
	ShaderTag mShaderTag;
};
