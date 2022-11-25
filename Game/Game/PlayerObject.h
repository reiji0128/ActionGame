#pragma once
#include "GameObject.h"
#include "IHitPointGetter.h"
#include <vector>
#include <unordered_map>
#include <functional>
#include "SkeletalMeshComponent.h"
#include "Math.h"
#include "ShaderTag.h"

// 前方宣言
class PlayerStateBase;
class PlayerStateIdle;
class PlayerStateRun;
class PlayerStateFirstAttack;
class PlayerStateSecondAttack;
class PlayerStateStandUp;
class PlayerStateDamage;
class PlayerStateFlyingBack;
class PlayerStateDeath;

class SkeletalMeshComponent;
class Animation;
class BoxCollider;
class LineCollider;
enum class DamageType;

// プレイヤーの状態
enum class PlayerState
{
	STATE_IDLE,           // 待機
	STATE_RUN,            // 走っている
	STATE_FIRST_ATTACK,   // 攻撃1
	STATE_SECOND_ATTACK,  // 攻撃2
	STATE_ROLL_FORWARD,   // 前転
	STATE_STAND_UP,       // 立ち上がり
	STATE_DAMAGE,         // ダメージ
	STATE_FLYING_BACK,    // 吹っ飛ばされ
	STATE_DEATH,          // 死亡

	STATE_NUM             // 総アニメーション数
};

class PlayerObject :public GameObject, public IHitPointGetter
{
public:

	PlayerObject(const Vector3& pos, const float& scale);

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
	void SetIsGround(bool IsOnGround) { mIsOnGround = IsOnGround; }

	void SetIsDamage(bool isDamage) { mIsDamage = isDamage; }

	void SetVelocity(const Vector3& velocity) { mVelocity = velocity; }

// ゲッター //

	int GetHP() const override { return mHP; }

	int GetMaxHP() const override { return mMaxHP; }

	int GetDamage(DamageType type) const;

	int GetReceiveDamage() const { return mDamage; }

	const Vector3& GetVelocity() const { return mVelocity; }

	/// <summary>
	/// 接地フラグのゲッター
	/// </summary>
	/// <returns>接地フラグ</returns>
	bool GetIsOnGround() { return mIsOnGround; }

	bool GetIsDamage() { return mIsDamage; }

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

	int mDamage;

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

	Vector3 mVelocity;

	// 攻撃リスト
	std::unordered_map<DamageType, int> mAttackList;

	// アニメーション可変長コンテナ
	std::vector<const Animation*> mAnimations;

	// ステートクラスプール
	std::vector<PlayerStateBase*> mStatePools;

	// スケルタルメッシュコンポーネントのポインタ
	SkeletalMeshComponent* mSkeltalMeshComp;

	// ボックスコライダーのポインタ
	BoxCollider* mBoxCollider;

	// ラインコライダーのポインタ
	LineCollider* mLineCollider;

	// 適用するシェーダーのタグ
	ShaderTag mShaderTag;

	// 各ステートクラスのポインタ
	PlayerStateIdle* mStateIdle;
	PlayerStateRun* mStateRun;
	PlayerStateFirstAttack* mStateFirstAttack;
	PlayerStateSecondAttack* mStateSecondAttack;
	PlayerStateStandUp* mStateStandUp;
	PlayerStateFlyingBack* mStateFlyingBack;
	PlayerStateDamage* mStateDamage;
	PlayerStateDeath* mStateDeath;
};
