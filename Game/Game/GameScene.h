#pragma once
#include "Game.h"
#include "SceneBase.h"
#include "Renderer.h"

//前方宣言
class StageCreator;
class BitmapText;
class Texture;
class DebugGrid;
class PlayerObject;
class EnemyObject;
class GameSceneUI;
class SwordObject;
class ThirdPersonCameraObject;
class EventNotificator;


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

	// ビットマップテキストのポインタ
	BitmapText* mFont;

	// テクスチャクラスのポインタ
	Texture* mTex;

	// デバッググリッドクラスのポインタ
	DebugGrid* mGrid;

	// ステージ作成クラスのポインタ
	StageCreator* mStageCreator;

	// ゲームシーンUIクラスのポインタ
	GameSceneUI* mGameSceneUI;

	PlayerObject* mPlayer;

	EnemyObject* mEnemy;

	SwordObject* mSword;

	ThirdPersonCameraObject* mCamera;

	EventNotificator* mEvent;
};