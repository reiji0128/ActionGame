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

	// �v���W�F�N�V�����s��̏�����
	InitProjMatrix();

	// ���C�g�̐ݒ�
	LightingSetting();

	// �t�H���g������
	mFont = new BitmapText;
	mFont->SetFontImage(16, 6, "Assets/Font/font.png");
	mFont->ReMapText(" !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_\tabcdefghijklmnopqrstuvwxyz{|}~\\");

	//�f�o�b�O�O���b�h
	Vector3 color(0, 1, 0);
	mGrid = new DebugGrid(2000, 20, color);

	mEvent = new EventNotificator();
	DamageNotificator::Initialize();

	// �v���C���[�̐���
	mPlayer = new PlayerObject(Vector3(990, -949, 110),1.0f);


	// ����̐���
	mSword = new SwordObject(Vector3(85, 5, 145),                       // �I�t�Z�b�g�ʒu
		                     Vector3(110, 0, 0),                        // �I�t�Z�b�g�p�x(radian)
		                     "Assets/Sword/SK_Sword.gpmesh",            // gpMesh�̃t�@�C���p�X
		                     mPlayer->GetSkeltalMeshComp(),             // �A�^�b�`���SkeltalMeshComp�N���X�̃|�C���^
		                     "LeftHandIndex4");                         // �A�^�b�`��̃{�[����

	// �G�̐���
	mEnemy = new EnemyObject(Vector3(3141, -949, 112.5f),               // ���W
		                     0.7f,                                      // �X�P�[��
		                     "Assets/Dragon/DRAGON.gpmesh",             // gpMesh�̃t�@�C���p�X
		                     "Assets/Dragon/DRAGON.gpskel");            // gpSkel�̃t�@�C���p�X

	// �J�����̐���
	mCamera = new ThirdPersonCameraObject(mPlayer);
	mCamera->SetCameraLength(400.0f);

	// �X�e�[�W�N���G�[�^�[�̐���
	mStageCreator = new StageCreator();

	// UI�̐���
	mGameSceneUI = new GameSceneUI(static_cast<IHitPointGetter*>(mPlayer), static_cast<IHitPointGetter*>(mEnemy));


	// BGM�̓ǂݍ��݁E�Đ�
	AUDIO->GetMusic("Assets/Sound/Battle.wav");
	AUDIO->PlayFadeInMusic("Assets/Sound/Battle.wav",3000);
	AUDIO->GetMusic("Assets/Sound/GameClear.wav");
	AUDIO->GetMusic("Assets/Sound/GameOver.wav");

	// ���ʉ��̓ǂݍ���
	AUDIO->GetSound("Assets/Sound/FireBall.wav");
	AUDIO->GetSound("Assets/Sound/FireBreath.wav");
	AUDIO->GetSound("Assets/Sound/PreliminaryAction01.wav");
	AUDIO->GetSound("Assets/Sound/PreliminaryAction02.wav");

	// �Q�[���V�X�e���ɓ����蔻�胊�X�g��o�^����
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

	// �����蔻��\�����[�h�̐؂�ւ�
	if (INPUT_INSTANCE.IsKeyPushdown(KEY_START))
	{
		GAMEINSTANCE.GetPhysics()->ToggleDebugMode();
	}

	DirectionalLight dirLight = RENDERER->GetDirectionalLight();
	Vector3 lightDir = dirLight.mDirection;
	RENDERER->SetDepthSetting(Vector3(1665.9f, -751.3f, 112.5f), lightDir, Vector3::UnitZ, mLightDistance);

	// �G�t�F�N�g�̍X�V����
	RENDERER->GetEffekseerManager()->Update();

	// UI�̍X�V����
	mGameSceneUI->Update(GAMEINSTANCE.GetDeltaTime());

	// �t�F�[�h�A�E�g���I����Ă�����^�C�g����ʂɑJ��
	if (mGameSceneUI->IsFinishFadeOut())
	{
		return new TitleScene;
	}

	return this;
}

void GameScene::Draw()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	// ��ʃN���A
	GAMEINSTANCE.GetRenderer()->WindowClear();

	// �Q�[���V�X�e���֘A�̕`��i�R���|�[�l���g�n�̂��̂͂����ł��ׂĕ`�悳���)
	GAMEINSTANCE.GetRenderer()->Draw();

// �G�t�F�N�g�֘A�̏��� //

	// �G�t�F�N�g�`��̊J�n����
	RENDERER->GetEffekseerRenderer()->BeginRendering();
	{
		RENDERER->GetEffekseerManager()->Draw();
	}
	// �G�t�F�N�g�`��̏I������
	RENDERER->GetEffekseerRenderer()->EndRendering();

	// 2D�`��̊J�n����
	RENDERER->SpriteDrawBegin();
	{
		mGameSceneUI->Draw();
	}
	// 2D�`��̏I������
	RENDERER->SpriteDrawEnd();


	// ��ʃt���b�v
	GAMEINSTANCE.GetRenderer()->WindowFlip();
}

void GameScene::InitProjMatrix()
{
	// �s�񏉊���
	Matrix4 projMat = Matrix4::CreatePerspectiveFOV(Math::ToRadians(75.0f),
		                                 GAMEINSTANCE.GetRenderer()->GetScreenWidth(),
		                                 GAMEINSTANCE.GetRenderer()->GetScreenHeight(),
		                                 1.0,
		                                 10000.0f);

	GAMEINSTANCE.GetRenderer()->SetProjMatrix(projMat);
}

void GameScene::LightingSetting()
{
	// ���C�e�B���O�̐ݒ�
	GAMEINSTANCE.GetRenderer()->SetAmbientLight(Vector3(0.1f, 0.1f, 0.1f));
	DirectionalLight& dir = GAMEINSTANCE.GetRenderer()->GetDirectionalLight();
	dir.mDirection    = Vector3(0.7f  , 0.7f  ,-0.7f);
	dir.mDiffuseColor = Vector3(0.78f , 0.88f , 1.0f);
	dir.mSpecColor    = Vector3(0.8f  , 0.8f  , 0.8f);
	Vector3 lightDir = dir.mDirection;

	RENDERER->SetDepthSetting(Vector3(1665.9f, -751.3f, 112.5f), lightDir, Vector3::UnitZ, mLightDistance);
}
