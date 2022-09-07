#pragma once
#include "Component.h"
#include <string>

class GameObject;
class Shader;
class Texture;

class CubeMapComponent : public Component
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="owner">ゲームオブジェクトのポインタ</param>
	CubeMapComponent(GameObject* owner);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~CubeMapComponent();

	/// <summary>
	/// テクスチャの生成
	/// </summary>
	/// <param name="fileName"></param>
	void CreateTexture(const std::string& fileName);

	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name="shader">使用するシェーダー</param>
	void Draw(Shader* shader);

// セッター　//

	/// <summary>
	/// 表示フラグのセット
	/// </summary>
	/// <param name="visible">
	/// true  : 表示する
	/// false : 表示しない
	/// </param>
	void SetVisible(bool visible) { mVisible = visible;}

// ゲッター //

	/// <summary>
	/// 表示フラグの取得
	/// </summary>
	/// <returns>
	/// true  : 表示する
	/// false : 表示しない
	/// </returns>
	bool GetVisible() const { return mVisible; }

private:
	// 表示フラグ
	bool mVisible;

	// テクスチャのポインタ
	Texture* mTexture;
};