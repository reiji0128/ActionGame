#include "ThirdPersonCameraObject.h"
#include "Game.h"
#include "Renderer.h"
#include "Input.h"

const float cameraLength = 1000.0f;
const float cameraHight = 350.0f;
const float rotateAngle = -Math::Pi;
const float upAngle = Math::PiOver2 * 0.5f;
const float rotateSpeed = 40.0f;

// 最大仰角
const float maxLookDownAngle = Math::ToRadians(70.0f);
// 最小仰角
const float minLookDownAngle = Math::ToRadians(20.0f);

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="targetObject">カメラが基準とするゲームオブジェクトのポインタ</param>
ThirdPersonCameraObject::ThirdPersonCameraObject(GameObject* targetObject)
	:CameraObject(targetObject)
	,mCameraLength(0.0f)
	,mRotateZAngle(0.0f)
	,mLookDownAngle()
{
	mTargetDir = targetObject->GetDirection();
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

	// 接近係数
	const float dumper = 1.5f;
	Vector3 diff;
	Vector3 playerPos = mTargetActor->GetPosition();

	// 差分ベクトルからプレイヤー位置まで近づける
	diff = playerPos - mViewTarget;
	diff = dumper * deltaTime * diff;

	// キー入力
	const float rotate = 0.5f * deltaTime;
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
	rotatePos.x = -mTargetDir.x * mCameraLength * cosf(mLookDownAngle) * cosf(mRotateZAngle);
	rotatePos.y = mCameraLength * cosf(mLookDownAngle) * sinf(mRotateZAngle);
	rotatePos.z = mCameraLength * sinf(mLookDownAngle);

	// 注視点・カメラ位置をセット
	mPosition = rotatePos + playerPos;
	mViewTarget = playerPos;

	// カメラ行列作成
	Matrix4 camMat = Matrix4::CreateLookAt(mPosition, mViewTarget, Vector3(0, 0, 1));
	RENDERER->SetViewMatrix(camMat);
}
