#include "GameOverScene.h"
#include "Texture.h"
#include "Input.h"
#include "TitleScene.h"
#include "GraphicResourceManager.h"

GameOverScene::GameOverScene()
{
	printf("---------------GameOverScene---------------\n");
	mGameOverTex = new Texture;
    mGameOverTex = GraphicResourceManager::GetTexture("Assets/Scene/GameOver.png");
}

GameOverScene::~GameOverScene()
{
}

SceneBase* GameOverScene::Update()
{
	if (INPUT_INSTANCE.IsKeyPushdown(KEY_START))
	{
		return new TitleScene;
	}
	return this;
}

void GameOverScene::Draw()
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
    RENDERER->DrawTexture(mGameOverTex, Vector2(960, 540), 1.0f, 1.0f);

    // 2D�`��̏I������
    RENDERER->SpriteDrawEnd();

    // ��ʂ̃t���b�v
    GAMEINSTANCE.GetRenderer()->WindowFlip();
}
