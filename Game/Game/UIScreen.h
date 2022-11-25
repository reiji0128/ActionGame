#pragma once
#include "Math.h"
#include <vector>

// 前方宣言
class Shader;
class Texture;

class UIScreen
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="offset">相対座標</param>
	/// <param name="scale">スケール</param>
	/// <param name="alpha">透過度</param>
	UIScreen(const Vector2& offset, const Vector2& scale, const float& alpha);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~UIScreen();

	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update(float deltaTime) {};

	/// <summary>
	/// 描画処理
	/// </summary>
	virtual void Draw() = 0;

protected:
	// 相対座標
	Vector2 mOffset;

	// スケール
	Vector2 mScale;

	// 透過度
	float mAlpha;

	// スクリーンの幅
	int mScreenWidth;

	// スクリーンの高さ
	int mScreenHeight;

	// Textureクラスのポインタ
	Texture* mTexture;
};
