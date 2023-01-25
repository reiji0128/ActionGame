#pragma once
#include "CameraObject.h"

class TargetSearch;

class ThirdPersonCameraObject : public CameraObject
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	ThirdPersonCameraObject(GameObject* targetObject, TargetSearch* targetSearch);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~ThirdPersonCameraObject();

	/// <summary>
	/// アクターの更新処理
	/// </summary>
	/// <param name="deltaTime">1フレームの経過時間</param>
	void UpdateGameObject(float deltaTime) override;

// セッター //

	/// <summary>
	/// Z軸の回転角のセット
	/// </summary>
	/// <param name="angle">Z軸の回転角</param>
	void SetRotateZAngle(float angle) { mRotateZAngle = angle; }

	/// <summary>
	/// 見下ろしの回転角のセット
	/// </summary>
	/// <param name="angle">見下ろしの回転角</param>
	void SetLookDownAngle(float angle) { mLookDownAngle = angle; }

	/// <summary>
	/// カメラの距離のセット
	/// </summary>
	/// <param name="length">カメラの距離</param>
	void SetCameraLength(float length) { mCameraLength = length; }

	/// <summary>
	/// カメラ座標のセット
	/// </summary>
	/// <param name="vec">カメラ座標</param>
	void SetCameraPos(Vector3& vec) { mCameraPos = vec; }

// ゲッター //

	/// <summary>
	/// カメラの前方ベクトルの取得
	/// </summary>
	/// <returns>カメラの前方ベクトル</returns>
	Vector3& GetCameraForwardVec() { return mCameraForwardVec; }

	/// <summary>
	/// カメラの座標の取得
	/// </summary>
	/// <returns>カメラの座標</returns>
	Vector3& GetCameraPos() { return mCameraPos; }

private:

	void TargetLockOn();

	// プレイヤーからの距離
	float mCameraLength;

	// カメラ周囲回転方向回転角
	float mRotateZAngle;

	// カメラ見下ろし方向回転角
	float mLookDownAngle;

	// めり込み防止の補正値
	const float mCorrectionValue;

	float mTime;

	// 前方ベクトル
	Vector3 mCameraForwardVec;

	// カメラ位置
	Vector3 mCameraPos;

	Vector3 mPlayerDir;

	// ターゲットロックをしているか
	bool mIsTargetLocke;

	// 補間が終わったか
	bool mIsFinishInterpolation;

	// ターゲットを探すクラス
	TargetSearch* mTargetSearch;
};
