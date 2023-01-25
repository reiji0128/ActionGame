#include "ThirdPersonCameraObject.h"
#include "Game.h"
#include "PhysicsWorld.h"
#include "Renderer.h"
#include "Input.h"
#include "Collision.h"
#include "TargetSearch.h"

const float rotateAngle = -Math::Pi;
const float upAngle = Math::PiOver2 * 0.5f;
const float rotateSpeed = 40.0f;

// 最大仰角
const float maxLookDownAngle = Math::ToRadians(70.0f);
// 最小仰角
const float minLookDownAngle = Math::ToRadians(00.0f);

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="targetObject">カメラが基準とするゲームオブジェクトのポインタ</param>
ThirdPersonCameraObject::ThirdPersonCameraObject(GameObject* targetObject, TargetSearch* targetSearch)
	:CameraObject(targetObject)
	,mCameraLength(0.0f)
	,mRotateZAngle(-Math::Pi)
	,mLookDownAngle(Math::PiOver2 * 0.35f)
	,mCorrectionValue(17.0f)
	,mIsTargetLocke(false)
	,mIsFinishInterpolation(true)
	,mTargetSearch(targetSearch)
	,mTime(0.0f)
{
	mPlayerDir = targetObject->GetDirection();
}

/// <summary>
/// デストラクタ
/// </summary>
ThirdPersonCameraObject::~ThirdPersonCameraObject()
{
}

/// <summary>
/// ゲームオブジェクトの更新処理
/// </summary>
/// <param name="deltaTime">1フレームの経過時間</param>
void ThirdPersonCameraObject::UpdateGameObject(float deltaTime)
{
	// プレイヤーが存在しない場合は処理しない
	if (!GAMEINSTANCE.IsExistGameObjectType(Tag::Player))
	{
		return;
	}

	Vector3 playerPos = mTargetObject->GetPosition();
	playerPos.z += 100;

	// キー入力
	const float rotate = 0.6f * deltaTime;
	if (INPUT_INSTANCE.IsKeyPressed(KEY_RIGHT))
	{
		mRotateZAngle += rotate;
	}
	if (INPUT_INSTANCE.IsKeyPressed(KEY_LEFT))
	{
		mRotateZAngle -= rotate;
	}
	if (INPUT_INSTANCE.IsKeyPressed(KEY_UP))
	{
		mLookDownAngle += rotate;
	}
	if (INPUT_INSTANCE.IsKeyPressed(KEY_DOWN))
	{
		mLookDownAngle -= rotate;
	}

	// パッド入力
	Vector2 axisR;
	axisR = INPUT_INSTANCE.GetRStick();
	mRotateZAngle += axisR.x * rotate;
	mLookDownAngle += axisR.y * rotate;
	// 見降ろし角度の角度制限
	if (mLookDownAngle < minLookDownAngle)
	{
		mLookDownAngle = minLookDownAngle;
	}
	if (mLookDownAngle > maxLookDownAngle)
	{
		mLookDownAngle = maxLookDownAngle;
	}

	// ヨー回転・ピッチ回転
	Vector3 rotatePos;
	rotatePos.x = mCameraLength * cosf(mLookDownAngle) * cosf(mRotateZAngle);
	rotatePos.y = mCameraLength * cosf(mLookDownAngle) * sinf(mRotateZAngle);
	rotatePos.z = mCameraLength * sinf(mLookDownAngle);

	// ロックオンされていなければ
	if (!mIsTargetLocke)
	{
		// 注視点・カメラ位置をセット
		mPosition = rotatePos + playerPos;
		mViewTarget = playerPos;
	}

	if (INPUT_INSTANCE.IsKeyPushdown(KEY_L))
	{
		// ロックオンされていなければロックオン
		if (!mIsTargetLocke)
		{
			mIsTargetLocke = true;
			mIsFinishInterpolation = false;

			// プレイヤーの近くの敵を検索
			mTargetSearch->SearchNearTargetPos(mTargetObject, true);
		}
		else
		{
			mIsTargetLocke = false;
			//mIsFinishInterpolation = false;
			mTime = 0.0f;
		}
	}

	// カメラのめり込み防止処理
	PhysicsWorld* phys = GAMEINSTANCE.GetPhysics();
	CollisionInfo info;

	Line line(playerPos, mPosition);
	
	// ラインが当たっていたら
	if (phys->SegmentCast(line, info))
	{
		// 衝突点にカメラを移動
		mPosition = info.mCollisionPoint;
		mPosition -= line.mNormalizeDirection * mCorrectionValue;
	}

	// ロックオンされていれば
	if (mIsTargetLocke)
	{
		TargetLockOn();
	}
	else
	{
		/*if (!mIsFinishInterpolation)
		{
			Vector3 startPos = mViewTarget;
			Vector3 endPos = mTargetObject->GetPosition();
			endPos.z += 100.0f;
			mViewTarget = Vector3::Lerp(startPos, endPos, mTime);
			printf("%f,%f,%f\n", mViewTarget.x, mViewTarget.y, mViewTarget.z);
			mTime += 0.01f;
			if (mTime > 1)
			{
				mIsFinishInterpolation = true;
			}
		}*/
	}
	
	// カメラ行列作成
	Matrix4 camMat = Matrix4::CreateLookAt(mPosition, mViewTarget, Vector3(0, 0, 1));
	RENDERER->SetViewMatrix(camMat);
	
}

void ThirdPersonCameraObject::TargetLockOn()
{
	// プレイヤーから敵に向かうベクトルの単位ベクトルを取得
	Vector3 enemyPos = mTargetSearch->GetLockeonTargetPos();
	Vector3 playerPos = mTargetObject->GetPosition();
	Vector3 playerToEnemyVec = enemyPos - playerPos;
	playerToEnemyVec.Normalize();

	// カメラ切り替えの補間が終わったか
	if (mIsFinishInterpolation)
	{
		// 注視点の更新
		mViewTarget = enemyPos + playerToEnemyVec;

		// カメラ座標の更新
		mPosition = playerPos - playerToEnemyVec * mCameraLength;
		mPosition.z = playerPos.z + mCameraLength;
	}
	else
	{
		// カメラ座標の更新
		mPosition = playerPos - playerToEnemyVec * mCameraLength;
		mPosition.z = playerPos.z + mCameraLength;
		playerPos.z += 100.0f;

		// 補間して注視点の更新
		mViewTarget = Vector3::Lerp(playerPos, enemyPos + playerToEnemyVec, mTime);
		mTime += 0.01f;

		if (mTime > 1)
		{
			mIsFinishInterpolation = true;
		}
	}

	// カメラのめり込み防止処理
	PhysicsWorld* phys = GAMEINSTANCE.GetPhysics();
	CollisionInfo info;

	Line line(playerPos, mPosition);

	// ラインが当たっていたら
	if (phys->SegmentCast(line, info))
	{
		// 衝突点にカメラを移動
		mPosition = info.mCollisionPoint;
		mPosition -= line.mNormalizeDirection * mCorrectionValue;
	}
}
