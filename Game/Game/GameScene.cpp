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
#include "IHitPointGetter.h"
#include "EventNotificator.h"
#include "GameSceneUI.h"
#include "PlayerObject.h"
#include "EnemyObject.h"
#include "SwordObject.h"
#include "ThirdPersonCameraObject.h"
#include "EventNotificator.h"
#include "DamageNotificator.h"
#include "TitleScene.h"

GameScene::GameScene()
	:mFont(nullptr)
	,mTex (nullptr)
	,mGrid(nullptr)
	,mLightDistance(4000.0f)
{
	SDL_Log("-----------------GameScene-----------------\n");

	// プロジェクション行列の初期化
	InitProjMatrix();

	// ライトの設定
	LightingSetting();

	// フォント初期化
	mFont = new BitmapText;
	mFont->SetFontImage(16, 6, "Assets/Font/font.png");
	mFont->ReMapText(" !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_\tabcdefghijklmnopqrstuvwxyz{|}~\\");

	//デバッググリッド
	Vector3 color(0, 1, 0);
	mGrid = new DebugGrid(2000, 20, color);

	mEvent = new EventNotificator();
	DamageNotificator::Initialize();

	// プレイヤーの生成
	mPlayer = new PlayerObject(Vector3(990, -949, 110),1.0f);


	// 武器の生成
	mSword = new SwordObject(Vector3(85, 5, 145),                       // オフセット位置
		                     Vector3(110, 0, 0),                        // オフセット角度(radian)
		                     "Assets/Sword/SK_Sword.gpmesh",            // gpMeshのファイルパス
		                     mPlayer->GetSkeltalMeshComp(),             // アタッチ先のSkeltalMeshCompクラスのポインタ
		                     "LeftHandIndex4");                         // アタッチ先のボーン名

	// 敵の生成
	mEnemy = new EnemyObject(Vector3(3141, -949, 112.5f),               // 座標
		                     0.7f,                                      // スケール
		                     "Assets/Dragon/DRAGON.gpmesh",             // gpMeshのファイルパス
		                     "Assets/Dragon/DRAGON.gpskel");            // gpSkelのファイルパス

	// カメラの生成
	mCamera = new ThirdPersonCameraObject(mPlayer);
	mCamera->SetCameraLength(400.0f);

	// ステージクリエーターの生成
	mStageCreator = new StageCreator();

	// UIの生成
	mGameSceneUI = new GameSceneUI(static_cast<IHitPointGetter*>(mPlayer), static_cast<IHitPointGetter*>(mEnemy));


	// BGMの読み込み・再生
	AUDIO->GetMusic("Assets/Sound/Battle.wav");
	AUDIO->PlayFadeInMusic("Assets/Sound/Battle.wav",3000);
	AUDIO->GetMusic("Assets/Sound/GameClear.wav");
	AUDIO->GetMusic("Assets/Sound/GameOver.wav");

	// 効果音の読み込み
	AUDIO->GetSound("Assets/Sound/FireBall.wav");
	AUDIO->GetSound("Assets/Sound/FireBreath.wav");
	AUDIO->GetSound("Assets/Sound/PreliminaryAction01.wav");
	AUDIO->GetSound("Assets/Sound/PreliminaryAction02.wav");

	// ゲームシステムに当たり判定リストを登録する
	GAMEINSTANCE.GetPhysics()->SetOneSideReactionCollisionPair(Tag::Enemy             , Tag::Player);
	GAMEINSTANCE.GetPhysics()->SetOneSideReactionCollisionPair(Tag::BackGround        , Tag::Player);
	GAMEINSTANCE.GetPhysics()->SetOneSideReactionCollisionPair(Tag::BackGround        , Tag::Enemy);
	GAMEINSTANCE.GetPhysics()->SetOneSideReactionCollisionPair(Tag::BackGround        , Tag::EnemyFireBall);
	GAMEINSTANCE.GetPhysics()->SetOneSideReactionCollisionPair(Tag::EnemyAttackBite   , Tag::Player);
	GAMEINSTANCE.GetPhysics()->SetOneSideReactionCollisionPair(Tag::EnemyAttackHand   , Tag::Player);
	GAMEINSTANCE.GetPhysics()->SetOneSideReactionCollisionPair(Tag::EnemyFireBreath   , Tag::Player);
	GAMEINSTANCE.GetPhysics()->SetOneSideReactionCollisionPair(Tag::EnemyFireBall     , Tag::Player);
	GAMEINSTANCE.GetPhysics()->SetOneSideReactionCollisionPair(Tag::PlayerFirstAttack , Tag::Enemy);
	GAMEINSTANCE.GetPhysics()->SetOneSideReactionCollisionPair(Tag::PlayerSecondAttack, Tag::Enemy);
	GAMEINSTANCE.GetPhysics()->SetOneSideReactionCollisionPair(Tag::Player            , Tag::EnemyFireBall);
	GAMEINSTANCE.GetPhysics()->SetOneSideReactionCollisionPair(Tag::EnemyFireBall     , Tag::Player);
}

GameScene::~GameScene()
{
	delete mPlayer;
	delete mEnemy;
	delete mSword;
	delete mCamera;
	delete mFont;
	delete mGrid;
	delete mStageCreator;
	delete mGameSceneUI;
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
	RENDERER->SetDepthSetting(Vector3(1665.9f, -751.3f, 112.5f), lightDir, Vector3::UnitZ, mLightDistance);

	// エフェクトの更新処理
	RENDERER->GetEffekseerManager()->Update();

	// UIの更新処理
	mGameSceneUI->Update(GAMEINSTANCE.GetDeltaTime());

	// フェードアウトが終わっていたらタイトル画面に遷移
	if (mGameSceneUI->IsFinishFadeOut())
	{
		return new TitleScene;
	}

	return this;
}

void GameScene::Draw()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	// 画面クリア
	GAMEINSTANCE.GetRenderer()->WindowClear();

	// ゲームシステム関連の描画（コンポーネント系のものはここですべて描画される)
	GAMEINSTANCE.GetRenderer()->Draw();

// エフェクト関連の処理 //

	// エフェクト描画の開始処理
	RENDERER->GetEffekseerRenderer()->BeginRendering();
	{
		RENDERER->GetEffekseerManager()->Draw();
	}
	// エフェクト描画の終了処理
	RENDERER->GetEffekseerRenderer()->EndRendering();

	// 2D描画の開始処理
	RENDERER->SpriteDrawBegin();
	{
		mGameSceneUI->Draw();
	}
	// 2D描画の終了処理
	RENDERER->SpriteDrawEnd();


	// 画面フリップ
	GAMEINSTANCE.GetRenderer()->WindowFlip();
}

void GameScene::InitProjMatrix()
{
	// 行列初期化
	Matrix4 projMat = Matrix4::CreatePerspectiveFOV(Math::ToRadians(75.0f),
		                                 GAMEINSTANCE.GetRenderer()->GetScreenWidth(),
		                                 GAMEINSTANCE.GetRenderer()->GetScreenHeight(),
		                                 1.0,
		                                 10000.0f);

	GAMEINSTANCE.GetRenderer()->SetProjMatrix(projMat);
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

	RENDERER->SetDepthSetting(Vector3(1665.9f, -751.3f, 112.5f), lightDir, Vector3::UnitZ, mLightDistance);
}
