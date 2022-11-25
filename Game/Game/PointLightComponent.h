#pragma once
#include "Component.h"
#include "Math.h"

// 前方宣言
class GameObject;
class Shader;
class Mesh;


class PointLightComponent : public Component
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="owner">コンポーネントを所有するゲームオブジェクト</param>
	/// <param name="offset">相対座標</param>
	/// <param name="color">ライトの色</param>
	/// <param name="luminance">輝度</param>
	PointLightComponent(GameObject* owner,const Vector3& offset, const Vector3& color,const float& luminance);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~PointLightComponent();

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="deltaTime">1フレームの経過時間</param>
	void Update(float deltaTime) override;

	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name="shader">使用するシェーダー</param>
	/// <param name="sphereMesh">描画に使う球体のメッシュ</param>
	void Draw(Shader* shader, Mesh* sphereMesh);

	/// <summary>
	/// ライトの減衰半径の計算
	/// </summary>
	/// <param name="constant">定数</param>
	/// <param name="linear">線形</param>
	/// <param name="quadratic">2乗項</param>
	void CalcAttenuationLightRadius(const float constant, const float linear, const float quadratic);

// セッター //

	/// <summary>
	/// 表示フラグのセット
	/// </summary>
	/// <param name="visibleFrag"></param>
	void SetIsVisible(bool visibleFrag) { mVisible = visibleFrag; }

	/// <summary>
	/// 色のセット
	/// </summary>
	/// <param name="color">色</param>
	void SetColor(Vector3 color) { mColor = color; }

private:

	// ライトの座標
	Vector3 mLightPos;

	// オフセット
	Vector3 mOffset;

	// ポイントライトの色
	Vector3 mColor;

	// ポイントライトの半径
	float mRadius;

	// 輝度
	float mLuminance;

	// 表示フラグ
	bool mVisible;
};
