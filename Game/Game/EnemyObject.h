#pragma once
#include "GameObject.h"
#include "IHitPointGetter.h"
#include <functional>
#include "ShaderTag.h"
#include <unordered_map>

// 前方宣言
class IHitPointGetter;
class EnemyStateBase;
class ColliderComponent;
class SkeletalMeshComponent;
class PointLightComponent;
class HitEffect;
class Animation;
class BoxCollider;
class LineCollider;
class FireBallObject;
enum class DamageType;

enum class EnemyState
{
	STATE_DETECTION,
	STATE_IDLE,
	STATE_PATROL,
	STATE_WALK,
	STATE_RUN,
	STATE_PRELIMINARY_ACTION_HAND,
	STATE_PRELIMINARY_ACTION_BITE,
	STATE_ATTACK_HAND,
	STATE_ATTACK_BITE,
	STATE_FIRE_BREATH,
	STATE_FIRE_BALL,
	STATE_DAMAGE,
	STATE_DEATH,

	STATE_NUM
};

class EnemyObject : public GameObject, public IHitPointGetter
{
public:

	EnemyObject(const Vector3& pos, const float& scale, const char* gpmeshFileName, const char* gpskelFileName);

	~EnemyObject();

	/// <summary>
	/// ゲームオブジェクトの更新処理
	/// </summary>
	/// <param name="deltaTime">1フレームの経過時間</param>
	void UpdateGameObject(float deltaTime) override;

	/// <summary>
	/// 衝突処理
	/// </summary>
	/// <param name="ownCollider">自分のコライダー</param>
	/// <param name="otherCollider">相手のコライダー</param>
	void OnCollisionEnter(ColliderComponent* ownCollider, ColliderComponent* otherCollider) override;

	void Damage(int damage);

	/// <summary>
	/// ボーン名からボーン行列を探す処理
	/// </summary>
	/// <param name="boneName">探すボーン行列</param>
	/// <returns>ボーン行列</returns>
	Matrix4 FindBoneMatrixFromName(const char* boneName);

	/// <summary>
	/// ライトの色の変更処理
	/// </summary>
	/// <param name="color"></param>
	void ChangeLightColor(Vector3 color);

// セッター //

	/// <summary>
	/// 加速度のセット
	/// </summary>
	/// <param name="velosity">加速度</param>
	void SetVelosity(const Vector3& velosity) { mVelocity = velosity; }

	/// <summary>
	/// ダメージフラグのセット
	/// </summary>
	/// <param name="isDamage"></param>
	void SetIsDamage(const bool isDamage) { mIsDamage = isDamage; }

	/// <summary>
	/// バッググラウンドフラグのセット
	/// </summary>
	/// <param name="isBackGround"></param>
	void SetIsBackGround(const bool isBackGround) { mIsBackGroundTrigger = isBackGround; }

// ゲッター //

	/// <summary>
	/// HPの取得
	/// </summary>
	/// <returns>現在のHP</returns>
	int GetHP() const override { return mHP; }

	/// <summary>
	/// 最大HPの取得
	/// </summary>
	/// <returns>最大HP</returns>
	int GetMaxHP() const override { return mMaxHP; }

	int GetReceiveDamage() const { return mDamage; }

	/// <summary>
	/// ダメージを受けているかの取得
	/// </summary>
	/// <returns></returns>
	bool GetIsDamage() const { return mIsDamage; }

	/// <summary>
	/// バッググラウンドと当たっているかどうかの取得
	/// </summary>
	/// <returns></returns>
	bool GetIsBackGround() const { return mIsBackGroundTrigger; }

	/// <summary>
	/// スケルタルメッシュコンポーネントのポインタの取得
	/// </summary>
	/// <returns>スケルタルメッシュコンポーネントのポインタ</returns>
	SkeletalMeshComponent* GetSkeletalMeshComp() { return mSkeltalMeshComp; }

	/// <summary>
	/// アニメーションの取得
	/// </summary>
	/// <param name="state">ステートに対応するアニメーション</param>
	/// <returns></returns>
	const Animation* GetAnim(EnemyState state) const { return mAnimations[static_cast<unsigned int>(state)]; }

	int GetDamage(DamageType type) const;

private:

	// 体力
	int mHP;

	// 最大体力
	const int mMaxHP = 100;

	int mDamage;

	// 無敵時間のタイマー
	float mEnvincibleTimer;

	// 無敵時間
	const float mEnvincibleTime;

	// ダメージを受けているか
	bool mIsDamage;

	// 無敵かどうか
	bool mIsEnvincible;

	// 無敵時間のタイマーをスタートするか
	bool mStartEnvincibleTimer;

	// バッググラウンドと当たっているか
	bool mIsBackGroundTrigger;

	// 加速度
	Vector3 mVelocity;

	// 現在の状態
	EnemyState mNowState;

	// 次の状態
	EnemyState mNextState;

	// 攻撃リスト
	std::unordered_map<DamageType, int> mAttackList;

	// アニメーションのコンテナ
	std::vector<const Animation*> mAnimations;

	// 状態のプール
	std::vector<EnemyStateBase*> mStatePools;

	// スケルタルメッシュコンポジットのポインタ
	SkeletalMeshComponent* mSkeltalMeshComp;

	// 当たり判定用ボックス
	BoxCollider* mBoxCollider;

	// 背景の判定トリガーボックス
	BoxCollider* mBackGroundTrigger;

	// 足元を調べるラインコライダー
	LineCollider* mLineCollider;

	// PointLightComponentのポインタ
	PointLightComponent* mCenterLight;
	PointLightComponent* mBehindLight;
	PointLightComponent* mFrontLight;
	PointLightComponent* mLeftLight;
	PointLightComponent* mRightLight;
	PointLightComponent* mFrontLeftLight;
	PointLightComponent* mFrontRightLight;

	// EffectComponentのポインタ
	HitEffect* mHitEffect;

	ShaderTag mUseShaderTag;
};