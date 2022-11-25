#pragma once
#include "UIScreen.h"

class IHitPointGetter;

class HitPointGauge : public UIScreen
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="offset">相対座標</param>
	/// <param name="scale">拡大率</param>
	/// <param name="alpha">透過度</param>
	/// <param name="textureFilePass">テクスチャのファイルパス</param>
	HitPointGauge(const Vector2& offset, const Vector2& scale, float alpha, const char* textureFilePass, IHitPointGetter* playerParameterGetter);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~HitPointGauge();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

	/// <summary>
	/// 体力ゲージのスケール計算
	/// </summary>
	void CalcGaugeScale();

private:
	// 現在の体力
	float mNowHP;
	
	// 最大体力
	float mMaxHP;

	float resultScale;

	// IPlayerParameterGetterクラスのポインタ
	IHitPointGetter* mHitPointGetter;
};