#pragma once
#include "Component.h"
#include "math.h"
#include <cstddef>
#include <Effekseer.h>

// 前方宣言
class GameObject;
class EffekseerEffect;

class EffectComponent : public Component
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="owner">ゲームオブジェクトのポインタ</param>
	/// <param name="moveOn">動くか?</param>
	/// <param name="rotateOn">回転するか?</param>
	/// <param name="updateOrder">更新順序</param>
	EffectComponent(GameObject* owner, bool moveOn = false, bool rotateOn = false, int updateOrder = 100);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~EffectComponent();

	/// <summary>
	/// エフェクトの読み込み
	/// </summary>
	/// <param name="effkseerFilename">エフェクトのファイルパス</param>
	void LoadEffect(const char16_t* effkseerFilename);

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="deltaTime"></param>
	void Update(float deltaTime) override;

	/// <summary>
	/// エフェクトの再生
	/// </summary>
	/// <param name="playRate">再生速度</param>
	void PlayEffect(const float& playRate = 1.0f);

	/// <summary>
	/// エフェクトの停止
	/// </summary>
	void StopEffect();

// セッター //

	/// <summary>
	/// 相対座標のセット
	/// </summary>
	/// <param name="relativePos">相対座標</param>
	void SetRelativePosition(const Vector3& relativePos) { mRelativePos = relativePos; }

	/// <summary>
	/// 相対回転行列のセット
	/// </summary>
	/// <param name="relativeRotMat">相対回転行列</param>
	void SetRelativeRotate(const Matrix4& relativeRotMat) { mRelativeRot = relativeRotMat; }

	/// <summary>
	/// 動くかどうかのセット
	/// </summary>
	/// <param name="move">
	/// true  : 動く
	/// false : 動かない
	/// </param>
	void SetMove(bool move) { mIsMove = move; }

	/// <summary>
	/// 回転するかどうかのセット
	/// </summary>
	/// <param name="rotate">
	/// true  : 回転する 
	/// false : 回転しない
	/// </param>
	void SetRotate(bool rotate) { mIsRotate = rotate; }

private:
	// エフェクトパラメータを管理するためのハンドル
	Effekseer::Handle mHandle;

	// 移動させるか?
	bool mIsMove;

	// 回転させるか?
	bool              mIsRotate;

	// 相対座標
	Vector3           mRelativePos;

	// 相対回転行列
	Matrix4           mRelativeRot;

	EffekseerEffect* mEffect;
};
