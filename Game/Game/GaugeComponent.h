#pragma once
#include "UIComponent.h"

class GaugeComponent : public UIComponent
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="owner">コンポーネントの所有者</param>
	/// <param name="drawOrder">描画順番</param>
	GaugeComponent(UIScreen* owner, Texture* texture, float maxValue, int drawOrderfloat = 100);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GaugeComponent();

	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name="shader">使用するシェーダー</param>
	void Draw(Shader* shader) override;

	/// <summary>
	/// 現在の値のセット
	/// </summary>
	/// <param name="nowValue">現在の値</param>
	void SetNowValue(float nowValue) { mNowValue = nowValue; }

	/// <summary>
	/// ゲージのスケール計算
	/// </summary>
	void CalcGaugeScale(const float& nowValue);

private:


	// 現在の値
	float mNowValue;

	// 最大の値
	float mMaxValue;

	// 最終的なスケール
	float mResultScale;
};