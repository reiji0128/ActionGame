#pragma once
#include "Game.h"
#include "SceneBase.h"
#include "Renderer.h"

class StageCreator;
class BitmapText;
class Texture;
class DebugGrid;
class PlayerHitPointBar;

class GameScene : public SceneBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

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

	/// <summary>
	/// プロジェクション行列の初期化
	/// </summary>
	void InitProjMatrix();

	void LightingSetting();

	float mLightDistance;

	float mHealthScaleX;

	// ビットマップテキストのポインタ
	BitmapText* mFont;

	// テクスチャクラスのポインタ
	Texture* mTex;

	// デバッググリッドクラスのポインタ
	DebugGrid* mGrid;

	//class Texture* mUI;

	StageCreator* mStageCreator;

	PlayerHitPointBar* mPlayerHitPointBar;
};