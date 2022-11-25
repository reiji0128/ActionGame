#include "TitleScene.h"
#include "Math.h"
#include "GameScene.h"
#include "DebugGrid.h"
#include "AudioManager.h"
#include "BitmapText.h"
#include "Input.h"
#include "Texture.h"
#include "GraphicResourceManager.h"
#include "TitleSprite.h"
#include "ButtonSprite.h"
#include "CurtainSprite.h"


/// <summary>
/// コンストラクタ
/// </summary>
TitleScene::TitleScene()
    :mTimer(0.0f)
    ,mTransitionTime(10.0f)
    ,mTitleSprite(nullptr)
    ,mButton(nullptr)
    ,mIsCount(false)
    ,mIsPush(false)
{
    printf("-----------------TitleScene-----------------\n");
    //フォントの初期化
    mFont = new BitmapText;
    mFont->SetFontImage(16, 6, "Assets/Font/font.png");
    mFont->ReMapText(" !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_\tabcdefghijklmnopqrstuvwxyz{|}~\\");

    mTitleSprite = new TitleSprite(Vector2(960, 540),
                                   Vector2(1, 1),
                                   1.0f,
                                   "Assets/UI/Title/Title.png");

    mButton      = new ButtonSprite(Vector2(950, 800),
                                     Vector2(1, 1),
                                     1.0f,
                                     "Assets/UI/Title/Button.png");

    mCurtain = new CurtainSprite(Vector2(960, 540),
                                 Vector2(1, 1),
                                 1.0f);

    // ボタンを押したときのサウンドの読み込み
    AUDIO->GetSound("Assets/Sound/Decision.wav");
    // タイトルBGMの読み込み
    AUDIO->GetMusic("Assets/Sound/Title.wav");
    AUDIO->PlayFadeInMusic("Assets/Sound/Title.wav", 3000);
}

/// <summary>
/// デストラクタ
/// </summary>
TitleScene::~TitleScene()
{
    delete mFont;
    delete mTitleSprite;
    delete mButton;
    delete mCurtain;
}

/// <summary>
/// シーンの更新処理
/// </summary>
/// <returns></returns>
SceneBase* TitleScene::Update()
{
    mCurtain->FadeIn(GAMEINSTANCE.GetDeltaTime());

    if (INPUT_INSTANCE.IsAnyButtonPushDown() && !mIsPush)
    {
        AUDIO->PlaySoundW("Assets/Sound/Decision.wav");
        mIsCount = true;
        mIsPush = true;
    }

    if (mIsCount)
    {
        mTimer += GAMEINSTANCE.GetDeltaTime();
        mCurtain->FadeOut(GAMEINSTANCE.GetDeltaTime());
        if (mTimer > mTransitionTime)
        {
            AUDIO->FadeOutMusic(2000);
            return new GameScene;
        }
    }

    mButton->Update(GAMEINSTANCE.GetDeltaTime());

    return this;
}

/// <summary>
/// シーンの描画処理
/// </summary>
void TitleScene::Draw()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // 画面のクリア
    GAMEINSTANCE.GetRenderer()->WindowClear();

    // ゲームシステム関連の描画(コンポーネント系のものはここですべて描画される
   // GAMEINSTANCE.GetRenderer()->Draw();

    // 2D関連の描画
    // 2D描画の準備処理
    RENDERER->SpriteDrawBegin();
    {
        mTitleSprite->Draw();
        mButton->Draw();
        mCurtain->Draw();
    }
    // 2D描画の終了処理
    RENDERER->SpriteDrawEnd();

    // 画面のフリップ
    GAMEINSTANCE.GetRenderer()->WindowFlip();
}
