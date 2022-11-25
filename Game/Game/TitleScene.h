#pragma once
#include "Game.h"
#include "SceneBase.h"
#include "Renderer.h"

// 前方宣言
class BitmapText;
class TitleSprite;
class ButtonSprite;
class CurtainSprite;

class TitleScene :public SceneBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	TitleScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~TitleScene();

	/// <summary>
	/// シーンの更新処理
	/// </summary>
	/// <returns></returns>
	SceneBase* Update();

	/// <summary>
	/// シーンの描画処理
	/// </summary>
	void Draw();

private:

	float mTimer;

	// 次の移行するまでの時間
	const float mTransitionTime;

	bool mIsCount;

	bool mIsPush;

	/// ビットマップテキストのポインタ
	BitmapText* mFont;

	TitleSprite* mTitleSprite;

	ButtonSprite* mButton;

	CurtainSprite* mCurtain;
};