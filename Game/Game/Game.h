#pragma once
#include <unordered_map>
#include <string>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_types.h>
#include "Math.h"
#include "Tag.h"
#include "GameObject.h"

// �O���錾
class Renderer;
class AudioManager;
class CameraObject;
class PhysicsWorld;
class SceneBase;
class GameObject;

class Game final
{
public:
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~Game();

	/// <summary>
	/// �Q�[�����s����
	/// </summary>
	void Run();

	/// <summary>
	/// �Q�[���I������
	/// </summary>
	void Shutdown();

	/// <summary>
	/// ��ʏ������ݒ�
	/// </summary>
	/// <param name="screenWidth">�X�N���[���̕�</param>
	/// <param name="screenHeight">�X�N���[���̍���</param>
	/// <param name="fullScreen">�t���X�N���[���ɂ��邩</param>
	/// <returns>
	/// true  : ����������
	/// false : ���������s
	/// </returns>
	bool Initialize(int screenWidth, int screenHeight, bool fullScreen = false);

// �Z�b�^�[ //
	/// <summary>
	/// �J�n�V�[���̃Z�b�g
	/// </summary>
	/// <param name="pScene">SceneBase�̃|�C���^</param>
	void SetFirstScene(SceneBase* pScene) { mNowScene = pScene; }

	/// <summary>
	/// �J�����I�u�W�F�N�g�̃|�C���̃^�Z�b�g
	/// </summary>
	/// <param name="camera">�Z�b�g����J�����I�u�W�F�N�g�̃|�C���^</param>
	void SetCameraObject(CameraObject* camera);

	/// <summary>
	/// �J�����I�u�W�F�N�g���A�N�e�B�u�ɃZ�b�g
	/// </summary>
	/// <param name="camera">�Z�b�g����J�����I�u�W�F�N�g�̃|�C���^</param>
	void SetInActiveCameraObject(CameraObject* camera);

	/// <summary>
	/// �Q�[�����V���b�g�_�E��
	/// </summary>
	void SetShutDown() { mIsRunning = false; }

// �Q�b�^�[ //

	/// <summary>
	/// �Q�[���N���X�̃C���X�^���X���擾
	/// </summary>
	/// <returns>�Q�[���N���X�̃C���X�^���X</returns>
	static Game& GetInstance()
	{
		static Game instance;
		return instance;
	}

	/// <summary>
	/// Renderer�|�C���^�̎擾
	/// </summary>
	/// <returns>Renderer�̃|�C���^</returns>
	Renderer* GetRenderer() { return mRenderer; }


	/// <summary>
	/// SDL�����_���[�̎擾
	/// </summary>
	SDL_Renderer* GetSDLRenderer();

	/// <summary>
	/// �f���^�^�C���̎擾
	/// </summary>
	/// <returns>�f���^�^�C��</returns>
	const float GetDeltaTime() { return mDeltaTime; }

	/// <summary>
	/// PhysicsWorld�N���X�̃|�C���^���擾
	/// </summary>
	/// <returns>PhysicsWorld�N���X�̃|�C���^</returns> 
	PhysicsWorld* const GetPhysics() { return mPhysicsWorld; }

	/// <summary>
	/// AudioManager�N���X�̃|�C���^���擾
	/// </summary>
	/// <returns>AudioManager�N���X�̃|�C���^</returns> 
	AudioManager* const GetAudio() { return mAudio; }

	/// <summary>
	/// �^�O����Q�[���I�u�W�F�N�g���擾
	/// </summary>
	/// <param name="type">�擾����Q�[���I�u�W�F�N�g�̃^�O</param>
	/// <returns>�Q�[���I�u�W�F�N�g�̃|�C���^</returns>
	std::vector<GameObject*> const& GetGameObjects(Tag type);

	/// <summary>
	/// �v���C���[�I�u�W�F�N�g�̃|�C���^�̎擾
	/// </summary>
	/// <returns>�v���C���[�I�u�W�F�N�g�̃|�C���^</returns>
	GameObject* GetPlayerObjects();

	/// <summary>
	/// �^�O�̒��ɂ���ŏ��̃Q�[���I�u�W�F�N�g��Ԃ�
	/// </summary>
	/// <returns>�ŏ��̃A�N�^�[�̃|�C���^</returns>
	GameObject* GetFirstGameObject(Tag type);

	/// <summary>
	/// �����_�̍��W�̎擾
	/// </summary>
	/// <returns>�����_�̍��W</returns>
	const Vector3& GetViewTargetPos();

	/// <summary>
	/// ���_�̍��W�̎擾
	/// </summary>
	/// <returns></returns>
	const Vector3& GetViewPos();

	/// <summary>
	/// �Q�[���I�u�W�F�N�g�̒ǉ�
	/// </summary>
	/// <param name="actor">�ǉ�����Q�[���I�u�W�F�N�g�̃|�C���^</param>
	void AddGameObject(GameObject* addObject);

	/// <summary>
	/// �Q�[���I�u�W�F�N�g�̍폜
	/// </summary>
	/// <param name="actor">�폜����Q�[���I�u�W�F�N�g�̃|�C���^</param>
	void RemoveGameObject(GameObject* removeObject);

	/// <summary>
	/// ���ׂẴQ�[���I�u�W�F�N�g�̍폜
	/// </summary>
	void RemoveAllGameObject();


	/// <summary>
	/// �^�C�v�̃Q�[���I�u�W�F�N�g�͑��݂��Ă��邩�H
	/// </summary>
	/// <param name="type">�m�F����A�N�^�[�̃^�O</param>
	/// <returns>�A�N�^�[�^�C�v�̃A�N�^�[�̐�</returns>
	bool IsExistGameObjectType(Tag type);

	/// <summary>
	/// �Q�[���I�u�W�F�N�gID����Q�[���I�u�W�F�N�g�ւ̃|�C���^����������
	/// </summary>
	/// <param name="searchActorID">��������Q�[���I�u�W�F�N�gID</param>
	/// <returns>
	/// �Y������ID�̃Q�[���I�u�W�F�N�g�ւ̃|�C���^
	/// </returns>
	GameObject* FindGameObjectFromID(int searchActorID);

private:

	/// <summary>
	/// �V���O���g��
	/// </summary>
	Game();

	/// <summary>
	/// ���͏���
	/// </summary>
	void Input();

	/// <summary>
	///  �`�揈��
	/// </summary>
	void Draw();

	/// <summary>
	/// �Q�[���S�̂̍X�V����
	/// </summary>
	int Update();

	/// <summary>
	/// �Q�[���I�u�W�F�N�g�̍X�V����
	/// </summary>
	void GameObjectUpdate();

	/// <summary>
	/// �Q�[���I�u�W�F�N�g���X�g�\���i�f�o�b�O�p�j
	/// </summary>
	void ShowGameObjectList();

private:
	// Renderer�N���X�̃|�C���^
	Renderer* mRenderer;

	// CameraObject�N���X�̃|�C���^
	CameraObject* mActiveCamera;

	// SceneBase�̃|�C���^
	SceneBase* mNowScene;

	// PhysicsWorld�N���X�̃|�C���^(�����蔻��V�X�e��)
	PhysicsWorld* mPhysicsWorld;

	// AudioManager�̃|�C���^(�T�E���h�֘A)
	AudioManager* mAudio;

	// �Q�[�����[�v�񂷂��H
	bool mIsRunning;

	// �|�[�Y���[�h
	bool mIsPauseMode;

	// 1�t���[���̌o�ߎ��ԁi�b�P�ʁj
	float mDeltaTime;

	// �o�ߎ��ԁi�~���b�P�ʁj
	Uint32 mTicksCount;

	// �ˉe�s��
	Matrix4 mViewMatrix;

	// �Q�[���I�u�W�F�N�g�ǉ������p�z��
	std::vector<GameObject*> mPendingGameObjects;

	// �Q�[���I�u�W�F�N�g�z��
	std::unordered_map<Tag, std::vector<GameObject*>> mGameObjects;
};


//Game�N���X�̃C���X�^���X
#define GAMEINSTANCE Game::GetInstance()

//Renderer�N���X�̃|�C���^���擾
#define RENDERER     Game::GetInstance().GetRenderer()

//AudioManager�N���X�̃|�C���^���擾
#define AUDIO        Game::GetInstance().GetAudio()
