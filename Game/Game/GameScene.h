﻿#pragma once
#include "Game.h"
#include "SceneBase.h"
#include "Renderer.h"

class StageCreator;

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
	void	   Draw();

private:
	void InitProjMatrix();

	void LightingSetting();

	float mLightDistance;

	float mHealthScaleX;

	/// ビットマップテキストのポインタ
	class BitmapText* mFont;

	/// テクスチャクラスのポインタ
	class Texture* mTex;

	/// デバッググリッドクラスのポインタ
	class DebugGrid* mGrid;

	//class Texture* mUI;

	StageCreator* mStageCreator;
};