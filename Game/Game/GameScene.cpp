#include "GameScene.h"
#include "Game.h"
#include <iostream>
#include "Math.h"
#include "Renderer.h"
#include "DebugGrid.h"
#include "AudioManager.h"
#include "BitmapText.h"
#include "Input.h"
#include "PhysicsWorld.h"
#include "Texture.h"
#include "StageCreator.h"


GameScene::GameScene()
	:mFont(nullptr)
	,mTex (nullptr)
	,mGrid(nullptr)
	,mHealthScaleX(0.3f)
	,mLightDistance(4000.0f)
{
	SDL_Log("-----------------GameScene-----------------\n");

	// フォント初期化
	mFont = new BitmapText;
	mFont->SetFontImage(16, 6, "Assets/Font/font.png");
	mFont->ReMapText(" !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_\tabcdefghijklmnopqrstuvwxyz{|}~\\");

	//デバッググリッド
	Vector3 color(0, 1, 0);
	mGrid = new DebugGrid(2000, 20, color);

	InitProjMatrix();

	LightingSetting();

	/*mTex = new Texture;
	mTex = RENDERER->GetTexture("Assets/Player/Health.png");*/                           

	// ステージクリエーターの生成
	mStageCreator = new StageCreator();

	// ゲームシステムに当たり判定リストを登録する
	GAMEINSTANCE.GetPhysics()->SetOneSideReactionCollisionPair(Tag::Enemy, Tag::Player);
	GAMEINSTANCE.GetPhysics()->SetOneSideReactionCollisionPair(Tag::BackGround, Tag::Player);
	GAMEINSTANCE.GetPhysics()->SetOneSideReactionCollisionPair(Tag::BackGround, Tag::Enemy);
}

GameScene::~GameScene()
{
	delete mFont;
	delete mGrid;
	delete mStageCreator;
}

SceneBase* GameScene::Update()
{
	static float time = 0;
	time += 0.01f;

	// 当たり判定表示モードの切り替え
	if (INPUT_INSTANCE.IsKeyPushdown(KEY_START))
	{
		GAMEINSTANCE.GetPhysics()->ToggleDebugMode();
	}

	DirectionalLight dirLight = RENDERER->GetDirectionalLight();
	Vector3 lightDir = dirLight.mDirection;
	RENDERER->SetDepthSetting(Vector3(890, -130, 20), lightDir, Vector3::UnitZ, mLightDistance);

	RENDERER->GetEffekseerManager()->Update();
	
	return this;
}

void GameScene::Draw()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	// 画面クリア
	GAMEINSTANCE.GetRenderer()->WindowClear();

	// ゲームシステム関連の描画（コンポーネント系のものはここですべて描画される)
	GAMEINSTANCE.GetRenderer()->Draw();

	// 2D描画の開始処理
	RENDERER->SpriteDrawBegin();

	

	// 2D描画の終了処理
	RENDERER->SpriteDrawEnd();

// エフェクト関連の処理 //
	// エフェクト描画の開始処理
	RENDERER->GetEffekseerRenderer()->BeginRendering();
	RENDERER->GetEffekseerManager()->Draw();
	// エフェクト描画の終了処理
	RENDERER->GetEffekseerRenderer()->EndRendering();

	// 画面フリップ
	GAMEINSTANCE.GetRenderer()->WindowFlip();
}

void GameScene::InitProjMatrix()
{
	// 行列初期化
	Matrix4 proj;
	proj = Matrix4::CreatePerspectiveFOV(Math::ToRadians(45.0f),
		                                 GAMEINSTANCE.GetRenderer()->GetScreenWidth(),
		                                 GAMEINSTANCE.GetRenderer()->GetScreenHeight(),
		                                 1.0,
		                                 10000.0f);

	GAMEINSTANCE.GetRenderer()->SetProjMatrix(proj);
}

void GameScene::LightingSetting()
{
	// ライティングの設定
	GAMEINSTANCE.GetRenderer()->SetAmbientLight(Vector3(0.1f, 0.1f, 0.1f));
	DirectionalLight& dir = GAMEINSTANCE.GetRenderer()->GetDirectionalLight();
	dir.mDirection    = Vector3(0.7f  , 0.7f  ,-0.7f);
	dir.mDiffuseColor = Vector3(0.78f , 0.88f , 1.0f);
	dir.mSpecColor    = Vector3(0.8f  , 0.8f  , 0.8f);
	Vector3 lightDir = dir.mDirection;

	RENDERER->SetDepthSetting(Vector3(1516, -1039, 112), lightDir, Vector3::UnitZ, mLightDistance);
}
