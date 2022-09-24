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

#include "PlayerStateIdle.h"
#include "PlayerStateRun.h"
#include "PlayerStateFirstAttack.h"


PlayerObject::PlayerObject(const Vector3& pos, const float& scale, const char* gpmeshFileName, const char* gpskelFileName)
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

	mHP = mMaxHP;

	// メッシュの読み込み・セット
	Mesh* mesh = GraphicResourceManager::LoadMesh(gpmeshFileName);
	mSkeltalMeshComp = new SkeletalMeshComponent(this,mShaderTag);
	mSkeltalMeshComp->SetMesh(mesh);

	// スケルトンの読み込み
	mSkeltalMeshComp->SetSkeleton(GraphicResourceManager::LoadSkeleton(gpskelFileName));
	// アニメーションの読み込み
	mAnimations.resize(static_cast<unsigned int>(PlayerState::STATE_NUM));
	mAnimations[static_cast<unsigned int>(PlayerState::STATE_IDLE)]         = GraphicResourceManager::LoadAnimation("Assets/Player/Great_Sword_Idle_Anim.gpanim"  , true);
	mAnimations[static_cast<unsigned int>(PlayerState::STATE_RUN)]          = GraphicResourceManager::LoadAnimation("Assets/Player/A_SW_Run.gpanim"               , true);
	mAnimations[static_cast<unsigned int>(PlayerState::STATE_FIRST_ATTACK)] = GraphicResourceManager::LoadAnimation("Assets/Player/Great_Sword_FirstSlash.gpanim" , true);
	
	// アイドル状態のアニメーションをセット
	mSkeltalMeshComp->PlayAnimation(mAnimations[static_cast<unsigned int>(PlayerState::STATE_IDLE)]);
	
	// アクターステートプールの初期化
	mStatePools.push_back(new PlayerStateIdle);
	mStatePools.push_back(new PlayerStateRun);
	mStatePools.push_back(new PlayerStateFirstAttack);

	// 当たり判定のセット
	AABB box = mesh->GetCollisionBox();
	box.Scaling(1.0f,1.0f,1.0f);
	mBoxCollider = new BoxCollider(this);
	mBoxCollider->SetObjectBox(box);

	// ラインコライダーのセット
	mLineCollider = new LineCollider(this);
	Line line(Vector3(0, 0, 50.0f), Vector3(0, 0, -20.0f));
	mLineCollider->SetLine(line);
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
}
