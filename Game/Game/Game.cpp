#include "Game.h"
#include "Input.h"
#include "Renderer.h"
#include "GraphicResourceManager.h"
#include "SceneBase.h"
#include "GameObject.h"
#include "Animation.h"
#include "Skeleton.h"
#include "PhysicsWorld.h"
#include "AudioManager.h"
#include "CameraObject.h"

#include <SDL.h>
#include <SDL_mixer.h>
#include <typeinfo>
#include <iostream>

/// <summary>
/// �V���O���g��
/// </summary>
Game::Game()
    :mRenderer(nullptr)
    ,mNowScene(nullptr)
    ,mAudio(nullptr)
    ,mIsRunning(true)
    ,mIsPauseMode(false)
    ,mPhysicsWorld(nullptr)
	,mActiveCamera(nullptr)
    ,mDeltaTime(0.0f)
    ,mTicksCount(0)
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Game::~Game()
{
}

/// <summary>
/// �Q�[�����s����
/// </summary>
void Game::Run()
{
	// �����_���[������������Ă��Ȃ������珉����
	if (!mRenderer)
	{
		Initialize(1920, 1080, true);
	}
	// �J�n�V�[������`����Ă��Ȃ�������I��
	if (!mNowScene)
	{
		printf("�J�n�V�[�����ݒ肳��Ă��܂���");
		return;
	}

	// �Q�[�����[�v
	while (mIsRunning)
	{
		Input();
		if (Update())
		{
			continue; // �V�[���؂�ւ����̂ݕ`�揈���������Ȃ�����
		}
		Draw();
	}
}

/// <summary>
/// �Q�[���I������
/// </summary>
void Game::Shutdown()
{
	RemoveAllGameObject();

	if (mRenderer)
	{
		mRenderer->Shutdown();
	}
}

/// <summary>
/// ����������
/// </summary>
/// <param name="screenWidth">�X�N���[���̕�</param>
/// <param name="screenHeight">�X�N���[���̍���</param>
/// <param name="fullScreen">�t���X�N���[���ɂ��邩</param>
/// <returns>
/// true  : ����������
/// false : ���������s
/// </returns>
bool Game::Initialize(int screenWidth, int screenHeight, bool fullScreen)
{
	// SDL������
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER) != 0)
	{
		printf("SDL���������s: %s\n", SDL_GetError());
		return false;
	}

	// �����_���[�̍쐬
	mRenderer = new Renderer;
	if (!mRenderer->Initialize(screenWidth, screenHeight, fullScreen))
	{
		printf("Renderer�̏������Ɏ��s");
		delete mRenderer;
		return false;
	}

	// �T�E���h�̏�����
	if (!Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG))
	{
		delete mRenderer;
		return false;
	}
	printf("SDLMixer����������\n");
	// SDLMixer API�������@44100:�����̎��g�� 2:�X�e���I 4096:�����o�b�t�@�T�C�Y
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) < 0)
	{
		delete mRenderer;
		Mix_CloseAudio();
		Mix_Quit();
		return false;
	}
	int decordNum = Mix_GetNumMusicDecoders();
	for (int i = 0; i < decordNum; ++i)
	{
		printf("MusicDecorder %d : %s\n", i, Mix_GetMusicDecoder(i));
	}
	int chunkNum = Mix_GetNumChunkDecoders();
	for (int i = 0; i < chunkNum; ++i)
	{
		printf("SoundDecorder %d : %s\n", i, Mix_GetChunkDecoder(i));
	}
	printf("SDLMixerAPI����������\n");

	// AudioSystem�쐬
	mAudio = new AudioManager;

	// �����蔻��V�X�e���쐬
	mPhysicsWorld = new PhysicsWorld;

	// �R���g���[��������
	INPUT_INSTANCE.ControllerInit();

	mTicksCount = SDL_GetTicks();
	return true;
}

void Game::SetCameraObject(CameraObject* camera)
{
	printf("SetCamera [%p]\n", camera);
	mActiveCamera = camera;
}

void Game::SetInActiveCameraObject(CameraObject* camera)
{
	if (camera == mActiveCamera)
	{
		printf("Camera is inActive. change to default view.\n");
		mActiveCamera = nullptr;
	}

	mViewMatrix = Matrix4::CreateLookAt(Vector3(0, 0, 100),
	                                 	Vector3(0, 0, 0),
		                                Vector3(0, 0, 1));
}

/// <summary>
/// 2D��SDL�����_���[�̎擾
/// </summary>
/// <returns>Renderer�̃|�C���^</returns>
SDL_Renderer* Game::GetSDLRenderer()
{
	return mRenderer->GetSDLRenderer();
}

/// <summary>
/// �^�O����Q�[���I�u�W�F�N�g���擾
/// </summary>
/// <param name="type">�Q�[���I�u�W�F�N�g�̃^�O</param>
/// <returns>�Ώۂ̃Q�[���I�u�W�F�N�g</returns>
std::vector<GameObject*> const& Game::GetGameObjects(Tag type)
{
	return mGameObjects[type];
}

/// <summary>
/// �v���C���[�I�u�W�F�N�g�̃|�C���^�̎擾
/// </summary>
/// <returns>�v���C���[�I�u�W�F�N�g�̃|�C���^</returns>
GameObject* Game::GetPlayerObjects()
{
	return GetFirstGameObject(Tag::Player);
}

/// <summary>
/// �^�O�̒��ɂ���ŏ��̃Q�[���I�u�W�F�N�g��Ԃ�
/// </summary>
/// <returns>�ŏ��̃Q�[���I�u�W�F�N�g�̃|�C���^</returns>
GameObject* Game::GetFirstGameObject(Tag type)
{
	// �A�N�e�B�u���X�g���`�F�b�N
	if (mGameObjects[type].size() != 0)
	{
		return mGameObjects[type][0];
	}

	// �y���f�B���O�ɂ��Ȃ���������
	for (auto iter = mPendingGameObjects.begin(); iter != mPendingGameObjects.end(); ++iter)
	{
		if ((*iter)->GetTag() == type)
		{
			return *iter;
		}
	}

	return nullptr;
}

const Vector3& Game::GetViewTargetPos()
{
	if (mActiveCamera == nullptr)
	{
		printf("Camera is inActive. return IllegalVec\n");
	}
	return mActiveCamera->GetViewTarget();
}

const Vector3& Game::GetViewPos()
{
	if (mActiveCamera == nullptr)
	{
		printf("Camera is inActive. return IllegalVec\n");
	}
	return mActiveCamera->GetViewPos();
}

/// <summary>
/// �Q�[���I�u�W�F�N�g�̒ǉ�
/// </summary>
/// <param name="addObject">�ǉ�����Q�[���I�u�W�F�N�g�̃|�C���^</param>
void Game::AddGameObject(GameObject* addObject)
{
	// ��������y���f�B���O�Q�[���I�u�W�F�N�g�ɒǉ�
	mPendingGameObjects.emplace_back(addObject);
}

/// <summary>
/// �Q�[���I�u�W�F�N�g�̍폜
/// </summary>
/// <param name="removeObject">�폜����Q�[���I�u�W�F�N�g</param>
void Game::RemoveGameObject(GameObject* removeObject)
{
	//�y���f�B���O�Q�[���I�u�W�F�N�g���ɂ���H
	auto iter = std::find(mPendingGameObjects.begin(), mPendingGameObjects.end(), removeObject);
	if (iter != mPendingGameObjects.end())
	{
		//�y���f�B���O�Q�[���I�u�W�F�N�g�̍Ō���Ƀf�[�^���ړ����āA�f�[�^������
		std::iter_swap(iter, mPendingGameObjects.end() - 1);
		mPendingGameObjects.pop_back();
		return;
	}

	// �A�N�e�B�u�Q�[���I�u�W�F�N�g���ɂ���H
	auto tag = removeObject->GetTag();
	iter = std::find(mGameObjects[tag].begin(), mGameObjects[tag].end(), removeObject);
	if (iter != mGameObjects[tag].end())
	{
		//�A�N�e�B�u�Q�[���I�u�W�F�N�g�̍Ō���Ƀf�[�^���ړ����āA�f�[�^����
		std::iter_swap(iter, mGameObjects[tag].end() - 1);
		mGameObjects[tag].pop_back();
	}
}

/// <summary>
/// ���ׂẴQ�[���I�u�W�F�N�g�̍폜
/// </summary>
void Game::RemoveAllGameObject()
{
	// �Q�[���I�u�W�F�N�g�̍폜�i�Q�[���I�u�W�F�N�g��ʂ��ăR���|�[�l���g���폜�����j
	for (auto tag = Tag::Begin; tag != Tag::End; ++tag)
	{
		while (!mGameObjects[tag].empty())
		{
			delete mGameObjects[tag].back();
		}
	}

	while (!mPendingGameObjects.empty())
	{
		delete mPendingGameObjects.back();
	}
}

/// <summary>
/// �^�C�v�̃Q�[���I�u�W�F�N�g�͑��݂��Ă��邩�H
/// </summary>
/// <param name="type">�Q�[���I�u�W�F�N�g�̃^�O</param>
/// <returns>�Q�[���I�u�W�F�N�g�̐�</returns>
bool Game::IsExistGameObjectType(Tag type)
{
	return mGameObjects[type].size() != 0;
}

/// <summary>
/// �Q�[���I�u�W�F�N�gID����Q�[���I�u�W�F�N�g�̃|�C���^����������
/// </summary>
/// <param name="searchActorID">��������ID</param>
/// <returns>�Q�[���I�u�W�F�N�g�̃|�C���^</returns>
GameObject* Game::FindGameObjectFromID(int searchActorID)
{
	// �A�N�e�B�u���X�g�����猟��
	for (Tag t = Tag::Begin; t != Tag::End; ++t)
	{
		for (auto item : mGameObjects[t])
		{
			if (item->GetID() == searchActorID)
			{
				return item;
			}
		}
	}
	// �y���f�B���O�ɂ��Ȃ���������
	for (auto iter = mPendingGameObjects.begin(); iter != mPendingGameObjects.end(); ++iter)
	{
		if ((*iter)->GetID() == searchActorID)
		{
			return *iter;
		}
	}
	return nullptr;
}

/// <summary>
/// ���͏���
/// </summary>
void Game::Input()
{
	// �I���C�x���g�̃L���b�`
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			mIsRunning = false;
			break;
		default:
			// �p�b�h�C�x���g�擾
			INPUT_INSTANCE.GetControllerInstance()->ReceiveEvent(event);
		}
	}

	// �L�[�{�[�h���͍X�V
	INPUT_INSTANCE.Update();

	// ESC�������ꂽ��I��
	if (INPUT_INSTANCE.IsKeyPullup(KEY_SELECT))
	{
		mIsRunning = false;
	}

	// �Q�[���I�u�W�F�N�g���X�g�̕\��
	if (INPUT_INSTANCE.IsKeyPullup(SDL_SCANCODE_F1))
	{
		ShowGameObjectList();
	}

	// �����_�����O���\�[�X�\��
	if (INPUT_INSTANCE.IsKeyPullup(SDL_SCANCODE_F2))
	{
		GraphicResourceManager::ShowResource();
	}

	// �|�[�Y���[�h�ڍs�^����
	if (INPUT_INSTANCE.IsKeyPullup(SDL_SCANCODE_BACKSPACE))
	{
		//�|�[�Y���[�h�ؑ�
		mIsPauseMode = !mIsPauseMode;
		GameObject::State changeState;

		if (mIsPauseMode)
		{
			changeState = GameObject::EPaused; //�|�[�Y
		}
		else
		{
			changeState = GameObject::EActive; //�A�N�e�B�u
		}

		//�S�ẴX�e�[�g��ύX����
		for (auto tag = Tag::Begin; tag != Tag::End; ++tag)
		{
			for (auto itr : mGameObjects[tag])
			{
				if (itr->GetState() != GameObject::EDead)
				{
					itr->SetState(changeState);
				}
			}
		}
	}
}

/// <summary>
/// �`�揈��
/// </summary>
void Game::Draw()
{
	if (!mNowScene)
	{
		return;
	}
	mNowScene->Draw();
}

/// <summary>
/// �X�V����
/// </summary>
int Game::Update()
{
	// 16�~���b(= 60�t���[��/�b)�ɂȂ�悤�ɁA�O�̃t���[�������16�~���b�ȏ�o�߂���܂ő҂�
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));

	// �O�̃t���[�����猻�ݎ����܂ł̌o�ߎ��ԎZ�o(�b�P�ʁj
	mDeltaTime = (SDL_GetTicks() - mTicksCount + 16) / 1000.0f;

	//���̃t���[���̊J�n���ԕۑ��i���̃t���[���܂ł̌o�ߎ��Ԃ��͂��邽�߁j
	mTicksCount = SDL_GetTicks();

	// �t���[�����Ԃ����܂�ɂ��o�߂��Ă���ꍇ��0.05 �� 20fps�Œ�
	if (mDeltaTime > 0.05f)
	{
		mDeltaTime = 0.05f;
	}

	if (!mNowScene)
	{
		return 0;
	}
	SceneBase* tmpScene;

	// �Q�[���I�u�W�F�N�g�̏���
	GameObjectUpdate();

	//�����蔻��
	mPhysicsWorld->Collision();

	//�V�[�������s
	tmpScene = mNowScene->Update();

	if (tmpScene != mNowScene)
	{
		delete mNowScene;

		//�V�K�V�[�������̃V�[���ɍ����ւ�
		mNowScene = tmpScene;

		return 1;
	}
	return 0;
}

/// <summary>
/// �Q�[���I�u�W�F�N�g�̍X�V����
/// </summary>
void Game::GameObjectUpdate()
{
	//�S�ẴQ�[���I�u�W�F�N�g�̍X�V
	for (auto tag = Tag::Begin; tag != Tag::End; ++tag)
	{
		for (auto actor : mGameObjects[tag])
		{
			actor->Update(mDeltaTime);
		}
	}

	//�y���f�B���O���̃A�N�^�[���A�N�e�B�u�A�N�^�[�Ɉړ�
	for (auto pending : mPendingGameObjects)
	{
		pending->ComputeWorldTransform();
		Tag t = pending->GetTag();

		mGameObjects[t].emplace_back(pending);
	}
	mPendingGameObjects.clear();

	// �S�Ă̎���ł���A�N�^�[���ꎞ�ۊ�
	std::vector<GameObject*> deadActors;
	for (auto tag = Tag::Begin; tag != Tag::End; ++tag)
	{
		for (auto actor : mGameObjects[tag])
		{
			if (actor->GetState() == GameObject::EDead)
			{
				deadActors.emplace_back(actor);
			}
		}
	}

	// ����ł���A�N�^�[��delete(mActor���������)
	for (auto actor : deadActors)
	{
		delete actor;
	}
	deadActors.clear();
}

/// <summary>
/// �Q�[���I�u�W�F�N�g���X�g�̕\��
/// </summary>
void Game::ShowGameObjectList()
{
	size_t actorCount = 0;
	for (Tag tag = Tag::Begin; tag != Tag::End; ++tag)
	{
		actorCount += mGameObjects[tag].size();
	}
	printf("\n\n<--------------GameObjectList----------------->\n");
	printf("---------> Active GameObject ( %zd ) <-----------\n", actorCount);

	for (auto tag = Tag::Begin; tag != Tag::End; ++tag)
	{
		for (auto i : mGameObjects[tag])
		{
			printf("mem [%p] : id: %d ", i, i->GetID());
			std::cout << typeid(*i).name() << "\n";
		}
	}

	printf("---------> Pending GameObject ( %zd ) <-----------\n", mPendingGameObjects.size());
	for (auto i : mPendingGameObjects)
	{
		printf("mem [%p] : id: %d ", i, i->GetID());
		std::cout << typeid(*i).name() << "\n";
	}
}
