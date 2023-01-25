#pragma once
#include "Math.h"
#include "UITag.h"

// 前方宣言
class UIScreen;
class Shader;
class Texture;

class UIComponent
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="owner">コンポーネントの所有者</param>
	/// <param name="drawingSequence">描画順番</param>
	UIComponent(UIScreen* owner, UITag tag, int drawOrder = 100);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~UIComponent();

	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name="shader"></param>
	virtual void Draw(Shader* shader) = 0;

// ゲッター //

	/// <summary>
	/// 描画順番の取得
	/// </summary>
	/// <returns>描画順番</returns>
	const int GetDrawOrder() const { return mDrawOrder; }

	/// <summary>
	/// 表示フラグの取得
	/// </summary>
	/// <returns>
	/// true  : 表示する
	/// false : 表示しない
	/// </returns>
	const bool GetVisible() const { return mVisible; }

	/// <summary>
	/// UIタグの取得
	/// </summary>
	/// <returns>UI1のタグ</returns>
	const UITag GetUITag() const { return mUITag; }

protected:

	// スクリーンの幅
	int mScreenWidth;

	// スクリーンの高さ
	int mScreenHeight;

	// 相対座標
	Vector2 mOffset;

	// スケール
	Vector2 mScale;

	// 透過度
	float mAlpha;

	// テクスチャ
	Texture* mTexture;

	// UIのタグ
	UITag mUITag;

	// 描画順番
	int mDrawOrder;

	// 表示するか
	bool mVisible;

	// コンポーネントの所有者
	UIScreen* mOwner;
};