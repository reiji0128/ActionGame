#pragma once
#include <unordered_map>
#include <string>
#include "ShaderTag.h"

class Shader;
class Texture;
class Mesh;
class Skeleton;
class Animation;
class EffekseerEffect;

class GraphicResourceManager final
{
public:
	/// <summary>
	/// �O���t�B�b�N���\�[�X�}�l�[�W���̏���������
	/// </summary>
	static void Initialize();

	/// <summary>
	/// �O���t�B�b�N���\�[�X�}�l�[�W���̏I������
	/// </summary>
	static void Finalize();

	/// <summary>
	/// �g�p����V�F�[�_�[�̌���
	/// </summary>
	/// <param name="findShaderTag">��������V�F�[�_�[�̃^�O</param>
	/// <returns>�^�O�ɑΉ�����V�F�[�_�[</returns>
	static Shader* FindUseShader(ShaderTag findShaderTag);

	/// <summary>
	/// �R���e�i�ɂ���e�N�X�`���E���b�V�����\�[�X�̕\���i�f�o�b�O�p�j
	/// </summary>
	static void ShowResource();

	/// <summary>
	/// �e�N�X�`���̎擾
	/// </summary>
	/// <param name="fileName">�e�N�X�`���̃t�@�C���p�X</param>
	/// <returns>�e�N�X�`���N���X�̃|�C���^</returns>
	static Texture* LoadTexture(const std::string& filePass);

	/// <summary>
	/// ���b�V���̎擾
	/// </summary>
	/// <param name="filePass">���b�V���̃t�@�C���p�X</param>
	/// <returns>���b�V���N���X�̃|�C���^</returns>
	static Mesh* LoadMesh(const std::string& filePass);

	/// <summary>
	/// �X�P���g���̎擾
	/// </summary>
	/// <param name="filePass">�X�P���g���t�@�C���p�X</param>
	/// <returns>�X�P���g���N���X�̃|�C���^</returns>
	const static Skeleton* LoadSkeleton(const char* filePass);

	/// <summary>
	/// �A�j���[�V�����̎擾
	/// </summary>
	/// <param name="filePass">�A�j���[�V�����̃t�@�C���p�X</param>
	/// <param name="isAnimationLoop">�A�j���[�V���������[�v�����邩�ǂ���</param>
	/// <returns>�A�j���[�V�����N���X�̃|�C���^</returns>
	const static Animation* LoadAnimation(const char* filePass, bool isAnimationLoop);


	static EffekseerEffect* GetEffect(const char16_t* filePass);

private:

	/// <summary>
	/// �V���O���g��
	/// </summary>
	GraphicResourceManager();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~GraphicResourceManager();

	/// <summary>
	/// �V�F�[�_�[�ǂݍ��ݏ���
	/// </summary>
	/// <returns>
	/// true  : �ǂݍ��ݐ���
	/// false : �ǂݍ��ݎ��s
	/// </returns>
	bool LoadShaders();

	/// <summary>
	/// �R���e�i�ɃV�F�[�_�[��ǉ�
	/// </summary>
	/// <param name="addShader">�ǉ�����V�F�[�_�[</param>
	/// <param name="addShaderTag">�ǉ�����V�F�[�_�[�̃^�O</param>
	void AddShader(Shader* addShader, ShaderTag addShaderTag);

	/// <summary>
	/// ���ׂẴV�F�[�_�[�̍폜
	/// </summary>
	void RemoveAllShader();

	/// <summary>
	/// ���ׂẴe�N�X�`���̍폜
	/// </summary>
	void RemoveAllTexture();

	/// <summary>
	/// ���ׂẴ��b�V���̍폜
	/// </summary>
	void RemoveAllMeshs();

	/// <summary>
	/// ���ׂẴX�P���g���̍폜
	/// </summary>
	void RemoveAllSkeletons();

	/// <summary>
	/// ���ׂẴA�j���[�V�����̍폜
	/// </summary>
	void RemoveAllAnims();

	/// <summary>
	/// ���ׂẴG�t�F�N�g�̍폜
	/// </summary>
	void RemoveAllEffects();

	// �}�l�[�W���̎��́i�A�v�����ɗB�ꑶ��)
	static GraphicResourceManager* mInstance;

	// �V�F�[�_�[���i�[����R���e�i
	std::unordered_map<ShaderTag, Shader*> mShaders;

	// �e�N�X�`�����i�[����R���e�i
	std::unordered_map<std::string, Texture*> mTextures;

	// ���b�V�����i�[����R���e�i
	std::unordered_map<std::string, Mesh*> mMeshs;

	// �X�P���g�����i�[����R���e�i
	std::unordered_map<std::string, Skeleton*> mSkeletons;

	// �A�j���[�V�������i�[����R���e�i
	std::unordered_map<std::string, Animation*> mAnims;

	// �G�t�F�N�g���i�[����R���e�i
	std::unordered_map<const char16_t*, class EffekseerEffect*> mEffects;
};