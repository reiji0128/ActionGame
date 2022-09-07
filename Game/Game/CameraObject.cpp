#include "CameraObject.h"
#include "Game.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="targetActor">カメラが基準とするゲームオブジェクトのポインタ</param>
CameraObject::CameraObject(GameObject* targetActor)
	:GameObject(Tag::Camera)
	,mTargetActor(targetActor)
{
	mPosition = Vector3(0, 100, 100);

	// デフォルトカメラ値セット
	mViewMatrix = Matrix4::CreateLookAt(Vector3(-100, 0, 100),
		                                Vector3(0, 0, 0),
		                                Vector3(0, 0, 1));

	// カメラをアクティブにセット
	SetActive();
}

CameraObject::~CameraObject()
{
	// ゲームシステム側に非アクティブを通知
	GAMEINSTANCE.SetInActiveCameraObject(this);
}

void CameraObject::Update(float deltaTime)
{
	UpdateGameObject(deltaTime);

	// カメラ視線ベクトル,カメラ行列作成
	mViewVector = mViewTarget - mPosition;
	mViewMatrix = Matrix4::CreateLookAt(mPosition,
		                                mViewTarget,
		                                Vector3(0, 0, 1));
}

void CameraObject::SetActive()
{
	// ゲームシステム側にアクティブを通知
	GAMEINSTANCE.SetCameraObject(this);
}

void CameraObject::Init(const Vector3& cameraPos, const Vector3& targetPos, const Vector3& upVec)
{
	mPosition = cameraPos;
	mViewTarget = targetPos;
	mViewVector = targetPos - cameraPos;

	mViewMatrix = Matrix4::CreateLookAt(cameraPos,
		                                targetPos,
		                                upVec);
}
