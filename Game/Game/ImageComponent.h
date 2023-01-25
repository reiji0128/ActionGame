#pragma once
#include "UIComponent.h"

class ImageComponent : public UIComponent
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="owner">コンポーネントの所有者</param>
	/// <param name="texture">テクスチャ</param>
	/// <param name="drawOrder">描画順番</param>
	ImageComponent(UIScreen* owner,Texture* texture, int drawOrder = 100);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~ImageComponent();

	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name="shader">使用するシェーダー</param>
	void Draw(Shader* shader) override;
};