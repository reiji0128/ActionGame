#include "PlayerObject.h"
#include "Game.h"
#include "Mesh.h"
#include "Skeleton.h"
#include "Renderer.h"
#include "GraphicResourceManager.h"
#include "BoxCollider.h"
#include "Animation.h"
#include "LineCollider.h"
#include "Input.h"
#include "ThirdPersonCameraObject.h"
#include "DamageNotificator.h"
#include "DamageType.h"

#include "PlayerStateIdle.h"
#include "PlayerStateRun.h"
#include "PlayerStateRollForward.h"
#include "PlayerStateFirstAttack.h"
#include "PlayerStateSecondAttack.h"
#include "PlayerStateStandUp.h"
#include "PlayerStateFlyingBack.h"
#include "PlayerStateDamage.h"
#include "PlayerStateDeath.h"


PlayerObject::PlayerObject(const Vector3& pos, const float& scale)
	:GameObject(Tag::Player)
	,mSkeltalMeshComp(nullptr)
	,mBoxCollider(nullptr)
	,mLineCollider(nullptr)
	,mBoneIndex(0)
	,mNowState()
	,mNextState()
	,mShaderTag(ShaderTag::SKINNED_SHADOW_MAP)
{
	// 座標のセット
	SetPosition(pos);

	// スケールのセット
	SetScale(scale);

	SetDirection(Vector3(1, 0, 0));

	mVelocity = Vector3(0, 0, 0);

	mHP = mMaxHP;

	// メッシュの読み込み・セット
	Mesh* mesh = GraphicResourceManager::LoadMesh("Assets/Player/Great_Sword_Idle.gpmesh");
	mSkeltalMeshComp = new SkeletalMeshComponent(this,mShaderTag);
	mSkeltalMeshComp->SetMesh(mesh);

	// スケルトンの読み込み
	mSkeltalMeshComp->SetSkeleton(GraphicResourceManager::LoadSkeleton("Assets/Player/Great_Sword_Idle.gpskel"));
	// アニメーションの読み込み
	mAnimations.resize(static_cast<unsigned int>(PlayerState::STATE_NUM));
	mAnimations[static_cast<unsigned int>(PlayerState::STATE_IDLE)]          = GraphicResourceManager::LoadAnimation("Assets/Player/Idle.gpanim"            , true);
	mAnimations[static_cast<unsigned int>(PlayerState::STATE_RUN)]           = GraphicResourceManager::LoadAnimation("Assets/Player/Run.gpanim"             , true);
	mAnimations[static_cast<unsigned int>(PlayerState::STATE_FIRST_ATTACK)]  = GraphicResourceManager::LoadAnimation("Assets/Player/FirstComboSlash.gpanim" , false);
	mAnimations[static_cast<unsigned int>(PlayerState::STATE_SECOND_ATTACK)] = GraphicResourceManager::LoadAnimation("Assets/Player/ThirdComboSlash.gpanim" , false);
	mAnimations[static_cast<unsigned int>(PlayerState::STATE_ROLL_FORWARD)]  = GraphicResourceManager::LoadAnimation("Assets/Player/Roll_Forward.gpanim", false);
	mAnimations[static_cast<unsigned int>(PlayerState::STATE_STAND_UP)]      = GraphicResourceManager::LoadAnimation("Assets/Player/Stand_Up.gpanim"        , false);
	mAnimations[static_cast<unsigned int>(PlayerState::STATE_DAMAGE)]        = GraphicResourceManager::LoadAnimation("Assets/Player/Damage.gpanim"          , false);
	mAnimations[static_cast<unsigned int>(PlayerState::STATE_FLYING_BACK)]   = GraphicResourceManager::LoadAnimation("Assets/Player/Flying_Back.gpanim"     , false);
	mAnimations[static_cast<unsigned int>(PlayerState::STATE_DEATH)]         = GraphicResourceManager::LoadAnimation("Assets/Player/Death.gpanim"           , false);
	
	// アイドル状態のアニメーションをセット
	mSkeltalMeshComp->PlayAnimation(mAnimations[static_cast<unsigned int>(PlayerState::STATE_IDLE)]);
	
	// アクターステートプールの初期化
	mStatePools.push_back(mStateIdle         = new PlayerStateIdle);
	mStatePools.push_back(mStateRun          = new PlayerStateRun);
	mStatePools.push_back(mStateFirstAttack  = new PlayerStateFirstAttack);
	mStatePools.push_back(mStateSecondAttack = new PlayerStateSecondAttack);
	mStatePools.push_back(new PlayerStateRollForward);
	mStatePools.push_back(mStateStandUp      = new PlayerStateStandUp);
	mStatePools.push_back(mStateDamage       = new PlayerStateDamage);
	mStatePools.push_back(mStateFlyingBack   = new PlayerStateFlyingBack);
	mStatePools.push_back(mStateDeath        = new PlayerStateDeath);

	// 当たり判定のセット
	AABB box = mesh->GetCollisionBox();
	box.Scaling(0.5f,1.7f,1.0f);
	box.mMin.z += 30;
	mBoxCollider = new BoxCollider(this);
	mBoxCollider->SetObjectBox(box);

	// ラインコライダーのセット
	mLineCollider = new LineCollider(this);
	Line line(Vector3(0, 0, 50.0f), Vector3(0, 0, -20.0f));
	mLineCollider->SetLine(line);


	// 攻撃のダメージのセット
	mAttackList[DamageType::PlayerFirstAttack] = 10;
	mAttackList[DamageType::PlayerSecondAttack] = 20;

	// ダメージ通知クラスに関数ポインタを追加
	DamageNotificator::AddDamageFunction(DamageType::PlayerFirstAttack, std::bind(&PlayerObject::GetDamage, this, std::placeholders::_1));
	DamageNotificator::AddDamageFunction(DamageType::PlayerSecondAttack, std::bind(&PlayerObject::GetDamage, this, std::placeholders::_1));

	
}

/// <summary>
/// デストラクタ
/// </summary>
PlayerObject::~PlayerObject()
{
}

/// <summary>
/// プレイヤーの更新処理
/// </summary>
/// <param name="deltaTime">1フレームの経過時間</param>
void PlayerObject::UpdateGameObject(float deltaTime)
{
	// ステート外部からステート変更があったか
	if (mNowState != mNextState)
	{
		mStatePools[static_cast<unsigned int>(mNextState)]->Enter(this, deltaTime);
		mNowState = mNextState;
		return;
	}
	
	// ステート実行
	mNextState = mStatePools[static_cast<unsigned int>(mNowState)]->Update(this, deltaTime);
	
	// ステート内部から変更があったか
	if (mNowState != mNextState)
	{
		mStatePools[static_cast<unsigned int>(mNextState)]->Enter(this, deltaTime);
		mNowState = mNextState;
	}

	mPosition += mVelocity;
}

/// <summary>
/// 衝突処理
/// </summary>
/// <param name="ownCollider"></param>
/// <param name="otherCollider"></param>
void PlayerObject::OnCollisionEnter(ColliderComponent* ownCollider, ColliderComponent* otherCollider)
{
	Tag colliderTag = otherCollider->GetTag();

	//衝突情報
	CollisionInfo info;

	// 背景と衝突した?
	if (colliderTag == Tag::BackGround)
	{
		// 衝突背景はBox?
		if (otherCollider->GetColliderType() == ColliderTypeEnum::Box)
		{
			// 背景Boxに衝突したのはLine?
			if (ownCollider == mLineCollider)
			{
				//落下しているときのみ判定
				if (mVelocity.z < 0.0f)
				{
					info = ownCollider->GetCollisionInfo();
					mPosition = info.mCollisionPoint;
					mIsOnGround = true;
				}
			}

			// 足元コライダー
			if (ownCollider == mLineCollider)
			{
				info = ownCollider->GetCollisionInfo();
				mPosition = info.mCollisionPoint;
				mIsOnGround = true;
				ComputeWorldTransform();
			}

			// 背景Boxに衝突したのもBox？
			if (ownCollider->GetColliderType() == ColliderTypeEnum::Box)
			{
				//ヒットボックス？
				if (ownCollider == mBoxCollider)
				{
					info = ownCollider->GetCollisionInfo();

					mPosition += info.mFixVec;
					ComputeWorldTransform();
				}
			}
		}
	}

	// エネミーとぶつかったか
	if (colliderTag == Tag::Enemy)
	{
		if (ownCollider == mBoxCollider)
		{
			info = ownCollider->GetCollisionInfo();
			mPosition += info.mFixVec;
			ComputeWorldTransform();
		}
	}


	if (colliderTag == Tag::EnemyAttackBite)
	{
		mIsDamage = true;
		mDamage = DamageNotificator::Norificate(DamageType::EnemyBiteAttack);
	}

	if (colliderTag == Tag::EnemyAttackHand)
	{
		mIsDamage = true;
		mDamage = DamageNotificator::Norificate(DamageType::EnemyHandAttack);
	}

	if (colliderTag == Tag::EnemyFireBall)
	{
		mIsDamage = true;
		mDamage = DamageNotificator::Norificate(DamageType::EnemyFireBall);
	}

	if (colliderTag == Tag::EnemyFireBreath)
	{
		mIsDamage = true;
		mDamage = DamageNotificator::Norificate(DamageType::EnemyFireBreath);
	}
}

void PlayerObject::Damage(int damage)
{
	mHP = mHP - damage;

	if (mHP <= 0)
	{
		mHP = 0;
	}

	mIsDamage = false;
}

int PlayerObject::GetDamage(DamageType type) const
{
	// ダメージの種類を探す
	auto iter = mAttackList.find(type);

	// 見つかったら対応するダメージを返す
	if (iter != mAttackList.end())
	{
		auto list = *iter;
		return list.second;
	}
}
