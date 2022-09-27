#pragma once
#include "HUD.h"

// 前方宣言
class VertexArray;

class PlayerHitPointBar : public HUD
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="offset">オフセット</param>
	/// <param name="scale">拡大率</param>
	/// <param name="alpha">透過度</param>
	PlayerHitPointBar(const Vector2& offset, const Vector2& scale, float alpha);

	~PlayerHitPointBar();

	void Draw() override;

private:
	VertexArray* mHitPointBarVerts;
};