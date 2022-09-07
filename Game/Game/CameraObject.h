#pragma once
#include "GameObject.h"

class CameraObject : public GameObject
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="targetActor">カメラが基準とするゲームオブジェクトのポインタ</param>
	CameraObject(GameObject* targetActor);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CameraObject();

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="deltaTime">1フレームの経過時間</param>
	void Update(float deltaTime);

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="cameraPos">カメラの座標</param>
	/// <param name="targetPos">ターゲットの座標</param>
	/// <param name="upVec">縦軸のベクトル</param>
	void Init(const Vector3& cameraPos, const Vector3& targetPos,
		      const Vector3& upVec);
	
// セッター //

	/// <summary>
	/// アクティブにセット
	/// </summary>
	void SetActive();

// ゲッター //

	/// <summary>
	/// ビュー行列の取得
	/// </summary>
	/// <returns>ビュー行列</returns>
	Matrix4& GetViewMatrix() { return mViewMatrix; }

	/// <summary>
	/// 視線ベクトルの取得
	/// </summary>
	/// <returns>視線ベクトル</returns>
	Vector3& GetViewVector() { return mViewVector; }

	/// <summary>
	/// ビューターゲットの座標の取得
	/// </summary>
	/// <returns>ビューターゲットの座標</returns>
	Vector3& GetViewTarget() { return mViewTarget; }

	/// <summary>
	/// カメラの座標の取得
	/// </summary>
	/// <returns>カメラの座標</returns>
	Vector3& GetViewPos() { return mPosition; }

protected:

	// カメラが基準とするゲームオブジェクトのポインタ
	GameObject* mTargetActor;

	// ビュー行列
	Matrix4 mViewMatrix;

	// カメラ注視点
	Vector3 mViewTarget;

	// 視線ベクトル
	Vector3 mViewVector;
};