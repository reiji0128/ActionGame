#include "GameClearScene.h"
#include "BitmapText.h"
#include "Texture.h"
#include "Input.h"
#include "TitleScene.h"
#include "GraphicResourceManager.h"

GameClearScene::GameClearScene()
{
	printf("--------------GameClearScene---------------\n");

    //�t�H���g�̏�����
    mFont = new BitmapText;
    mFont->SetFontImage(16, 6, "Assets/Font/font.png");
    mFont->ReMapText(" !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_\tabcdefghijklmnopqrstuvwxyz{|}~\\");

    mGameClearTex = new Texture;
    mGameClearTex = GraphicResourceManager::LoadTexture("Assets/Scene/GameClear.png");
}

GameClearScene::~GameClearScene()
{
}

SceneBase* GameClearScene::Update()
{
    if (INPUT_INSTANCE.IsKeyPushdown(KEY_START))
    {
        return new TitleScene;
    }
    return this;
}

void GameClearScene::Draw()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // ��ʂ̃N���A
    GAMEINSTANCE.GetRenderer()->WindowClear();

    // �Q�[���V�X�e���֘A�̕`��(�R���|�[�l���g�n�̂��̂͂����ł��ׂĕ`�悳���
    GAMEINSTANCE.GetRenderer()->Draw();

    // 2D�֘A�̕`��
    // 2D�`��̏�������
    RENDERER->SpriteDrawBegin();

    // 2DSprite��t�H���g�������ɏ���
    RENDERER->DrawTexture(mGameClearTex, Vector2(960, 540), 1.0f, 1.0f);

    // 2D�`��̏I������
    RENDERER->SpriteDrawEnd();

    // ��ʂ̃t���b�v
    GAMEINSTANCE.GetRenderer()->WindowFlip();
}
