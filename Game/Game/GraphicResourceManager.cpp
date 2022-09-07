#include "GraphicResourceManager.h"
#include "Shader.h"
#include "Texture.h"
#include "Mesh.h"
#include "Skeleton.h"
#include "Animation.h"
#include "EffekseerEffect.h"

// GraphicResource���̂ւ̃|�C���^��`
GraphicResourceManager* GraphicResourceManager::mInstance = nullptr;

GraphicResourceManager::GraphicResourceManager()
{
	mInstance = nullptr;
}

GraphicResourceManager::~GraphicResourceManager()
{
}

void GraphicResourceManager::Initialize()
{
	if (!mInstance)
	{
		mInstance = new GraphicResourceManager;

		mInstance->LoadShaders();
	}
}

void GraphicResourceManager::Finalize()
{
	mInstance->RemoveAllShader();
	mInstance->RemoveAllTexture();
	mInstance->RemoveAllMeshs();
	mInstance->RemoveAllSkeletons();
	mInstance->RemoveAllEffects();
}

Shader* GraphicResourceManager::FindUseShader(ShaderTag findShaderTag)
{
	auto shader = mInstance->mShaders.find(findShaderTag);

	// �R���e�i�̒��Ɍ������Ă���V�F�[�_�[�̃^�O������ΑΉ�����V�F�[�_�[��Ԃ�
	// ������Ȃ���� nullptr ��Ԃ�
	if (shader != mInstance->mShaders.end())
	{
		return mInstance->mShaders[findShaderTag];
	}
	else
	{
		printf("�V�F�[�_�[��������܂���ł���\n");
		return nullptr;
	}
}

void GraphicResourceManager::ShowResource()
{
	printf("\n\n<------------------ Textures ------------------>\n");
	for (auto i : mInstance->mTextures)
	{
		printf("texfile %s\n", i.first.c_str());
	}

	printf("\n<------------------  Meshes  ------------------->\n");
	for (auto i : mInstance->mMeshs)
	{
		printf("meshfile %s\n", i.first.c_str());
	}

	printf("\n<------------------  Skeletons  ------------------->\n");
	for (auto i : mInstance->mSkeletons)
	{
		printf("meshfile %s\n", i.first.c_str());
	}

	printf("\n<------------------  Animations  ------------------->\n");
	for (auto i : mInstance->mAnims)
	{
		printf("meshfile %s\n", i.first.c_str());
	}
}

Texture* GraphicResourceManager::GetTexture(const std::string& filePass)
{
	Texture* tex = nullptr;
	auto iter = mInstance->mTextures.find(filePass);

	// �R���e�i�̒��ɂ��̃e�N�X�`��������ꍇ���̃e�N�X�`����Ԃ�
	// �Ȃ���΃R���e�i�ɒǉ����ăe�N�X�`����Ԃ�
	if (iter != mInstance->mTextures.end())
	{
		tex = iter->second;
	}
	else
	{
		tex = new Texture();
		
		if (tex->Load(filePass))
		{
			mInstance->mTextures.emplace(filePass, tex);
		}
		else
		{
			delete tex;
			tex = nullptr;
		}
	}
	printf(" Load Texture Success : %s \n", filePass.c_str());

	return tex;
}

Mesh* GraphicResourceManager::GetMesh(const std::string& filePass)
{
	Mesh* mesh = nullptr;
	auto iter = mInstance->mMeshs.find(filePass);

	// �R���e�i�̒��ɂ��̃��b�V��������ꍇ���̃��b�V����Ԃ�
	// �Ȃ���΃R���e�i�ɒǉ����ă��b�V����Ԃ�
	if (iter != mInstance->mMeshs.end())
	{
		mesh = iter->second;
	}
	else
	{
		mesh = new Mesh;

		if (mesh->Load(filePass))
		{
			mInstance->mMeshs.emplace(filePass, mesh);
		}
		else
		{
			delete mesh;
			mesh = nullptr;
		}
	}
	printf(" Load Mesh Success : %s\n", filePass.c_str());
	
	return mesh;
}

const Skeleton* GraphicResourceManager::GetSkeleton(const char* filePass)
{
	Skeleton* sk = nullptr;
	std::string file(filePass);
	auto iter = mInstance->mSkeletons.find(file);

	// �R���e�i�̒��ɂ��̃X�P���g��������ꍇ���̃X�P���g����Ԃ�
	// �Ȃ���΃R���e�i�ɃX�P���g����ǉ����Ă�Ԃ�
	if (iter != mInstance->mSkeletons.end())
	{
		return iter->second;
	}
	else
	{
		sk = new Skeleton();
		if (sk->Load(file))
		{
			mInstance->mSkeletons.emplace(file, sk);
		}
		else
		{
			delete sk;
			sk = nullptr;
		}
		return sk;
	}
}

const Animation* GraphicResourceManager::GetAnimation(const char* filePass, bool isAnimationLoop)
{
	Animation* anim = nullptr;
	auto iter = mInstance->mAnims.find(filePass);

	// �R���e�i�̒��ɂ��̃A�j���[�V����������ꍇ���̃A�j���[�V������Ԃ�
	// �Ȃ���΃R���e�i�ɃA�j���[�V������ǉ����Ă�Ԃ�
	if (iter != mInstance->mAnims.end())
	{
		return iter->second;
	}
	else
	{
		anim = new Animation();
		if (anim->Load(filePass, isAnimationLoop))
		{
			mInstance->mAnims.emplace(filePass, anim);
		}
		else
		{
			delete anim;
			anim = nullptr;
		}
		return anim;
	}
}

EffekseerEffect* GraphicResourceManager::GetEffect(const char16_t* filePass)
{
	EffekseerEffect* effect = nullptr;
	auto iter = mInstance->mEffects.find(filePass);

	// �R���e�i�̒��ɂ��̃G�t�F�N�g������ꍇ���̃G�t�F�N�g��Ԃ�
	// �Ȃ���΃R���e�i�ɃG�t�F�N�g��ǉ����Ă�Ԃ�
	if (iter != mInstance->mEffects.end())
	{
		return iter->second;
	}
	else
	{
		effect = new EffekseerEffect;
		if (effect->LoadEffect(filePass))
		{
			mInstance->mEffects.emplace(filePass, effect);
		}
		else
		{
			delete effect;
			effect = nullptr;
		}
	}
	return effect;
}

bool GraphicResourceManager::LoadShaders()
{
	Shader* shader = nullptr;

	// �X�v���C�g�V�F�[�_�[�̓ǂݍ���
	shader = new Shader();
	if (!shader->Load("Shaders/Sprite.vert", "Shaders/Sprite.frag"))
	{
		printf("�X�v���C�g�V�F�[�_�[�̓ǂݍ��ݎ��s\n");
		return false;
	}
	AddShader(shader, ShaderTag::SPRITE);


	// �^�C���}�b�v�V�F�[�_�[�̓ǂݍ���
	shader = new Shader();
	if (!shader->Load("Shaders/Sprite.vert", "Shaders/Tilemap.frag"))
	{
		printf("�^�C���}�b�v�V�F�[�_�[�̓ǂݍ��ݎ��s\n");
		return false;
	}
	AddShader(shader, ShaderTag::TILEMAP);


	// ���b�V���p�f�v�X�V�F�[�_�[�̓ǂݍ���
	shader = new Shader();
	if (!shader->Load("Shaders/DepthMap.vert", "Shaders/DepthMap.frag"))
	{
		printf("���b�V���̃f�v�X�V�F�[�_�[�̓ǂݍ��ݎ��s\n");
		return false;
	}
	AddShader(shader, ShaderTag::DEPTH_MAP);


	// �X�L�����b�V���p�f�v�X�V�F�[�_�[�̓ǂݍ���
	shader = new Shader();
	if (!shader->Load("Shaders/SkinnedDepthMap.vert", "Shaders/DepthMap.frag"))
	{
		printf("�X�L�����b�V���̃f�v�X�V�F�[�_�[�̓ǂݍ��ݎ��s\n");
		return false;
	}
	AddShader(shader, ShaderTag::SKINNED_DEPTH_MAP);


	// ���b�V���p�V���h�E�}�b�v�V�F�[�_�[�̓ǂݍ���
	shader = new Shader();
	if (!shader->Load("Shaders/Shadowmap.vert", "Shaders/Shadowmap.frag"))
	{
		printf("�V���h�E�}�b�v�V�F�[�_�[�̓ǂݍ��ݎ��s\n");
		return false;
	}
	AddShader(shader, ShaderTag::SHADOW_MAP);


	// �X�L�����b�V���V�F�[�_�[�̓ǂݍ���
	shader = new Shader();
	if (!shader->Load("Shaders/SkinnedShadowmap.vert", "Shaders/Shadowmap.frag"))
	{
		printf("�X�L�����b�V���V�F�[�_�̓ǂݍ��ݎ��s\n");
		return false;
	}
	AddShader(shader, ShaderTag::SKINNED_SHADOW_MAP);


	// HDR�V�F�[�_�[�̓ǂݍ���
	shader = new Shader();
	if (!shader->Load("Shaders/HighLightMesh.vert", "Shaders/HighLightMesh.frag"))
	{
		printf("�X�t�B�A�V�F�[�_�̓ǂݍ��ݎ��s\n");
		return false;
	}
	AddShader(shader, ShaderTag::HDR);

	// �_�E���T���v�����O�V�F�[�_�[�̓ǂݍ���
	shader = new Shader();
	if (!shader->Load("Shaders/ScreenBuffer.vert", "Shaders/DownSampling.frag"))
	{
		printf("�_�E���T���v�����O�V�F�[�_�[�̓ǂݍ��ݎ��s\n");
	}
	AddShader(shader, ShaderTag::DOWNSAMPLING);


	// �K�E�X�u���[�V�F�[�_�[�̓ǂݍ���
	shader = new Shader();
	if (!shader->Load("Shaders/ScreenBuffer.vert", "Shaders/GaussBlur.frag"))
	{
		printf("�K�E�X�u���[�V�F�[�_�[�̓ǂݍ��ݎ��s\n");
	}
	AddShader(shader, ShaderTag::GAUSS_BLUR);


	// HDRBloom�u�����h�V�F�[�_�[�̓ǂݍ���
	shader = new Shader();
	if (!shader->Load("Shaders/ScreenBuffer.vert", "Shaders/HDRBloomBlend.frag"))
	{
		printf("HDRBloom�u�����h�V�F�[�_�[�̓ǂݍ��ݎ��s\n");
	}
	AddShader(shader, ShaderTag::HDR_BLOOM_BLEND);


	// �X�N���[���o�b�t�@�[�V�F�[�_�[�̓ǂݍ���
	shader = new Shader();
	if (!shader->Load("Shaders/ScreenBuffer.vert", "Shaders/HDR.frag"))
	{
		printf("�X�N���[���o�b�t�@�[�V�F�[�_�[�̓ǂݍ��ݎ��s\n");
	}
	AddShader(shader, ShaderTag::SCREEN_BUFFER);


	// �@���}�b�v�V�F�[�_�[�̓ǂݍ���
	shader = new Shader();
	if (!shader->Load("Shaders/Normal.vert", "Shaders/Normal.frag"))
	{
		printf("�@���}�b�v�V�F�[�_�̓ǂݍ��ݎ��s\n");
		return false;
	}
	AddShader(shader, ShaderTag::NORMAL_MAP);


	// �X�J�C�{�b�N�X�V�F�[�_�[�̓ǂݍ���
	shader = new Shader();
	if (!shader->Load("Shaders/SkyBox.vert", "Shaders/SkyBox.frag"))
	{
		printf("�X�J�C�{�b�N�X�V�F�[�_�[�̓ǂݍ��ݎ��s\n");
		return false;
	}
	AddShader(shader, ShaderTag::SKYBOX);

	return true;
}

void GraphicResourceManager::AddShader(Shader* addShader, ShaderTag addShaderTag)
{
	auto shader = mShaders.find(addShaderTag);

	// �R���e�i�̒��ɒǉ�����V�F�[�_�[�����łɂ���Βǉ����Ȃ�
	// �V�F�[�_�[���Ȃ���΃R���e�i�ɒǉ�
	if (shader != mShaders.end())
	{
		return;
	}
	else
	{
		mShaders[addShaderTag] = addShader;
	}
}

void GraphicResourceManager::RemoveAllShader()
{
	for (auto shader : mShaders)
	{
		shader.second->Unload();
		delete shader.second;
	}
	mShaders.clear();
}

void GraphicResourceManager::RemoveAllTexture()
{
	//�e�N�X�`���j��
	for (auto tex : mTextures)
	{
		printf("Texture  Release : %s\n", tex.first.c_str());
		tex.second->Unload();
		delete tex.second;
	}
	mTextures.clear();
}

void GraphicResourceManager::RemoveAllMeshs()
{
	// ���b�V���j��
	for (auto mesh : mMeshs)
	{
		printf("Mesh     Release : %s\n", mesh.first.c_str());
		mesh.second->Unload();
		delete mesh.second;
	}
	mMeshs.clear();
}

void GraphicResourceManager::RemoveAllSkeletons()
{
	// �X�P���g���̔j��
	for (auto skeleton : mSkeletons)
	{
		printf("Skeleton Release : %s\n", skeleton.first.c_str());
		delete skeleton.second;
	}
	mSkeletons.clear();
}

void GraphicResourceManager::RemoveAllAnims()
{
	// �A�j���[�V�����̔j��
	for (auto anim : mAnims)
	{
		printf("Animation Release : %s\n", anim.first.c_str());
		delete anim.second;
	}
	mAnims.clear();
}

void GraphicResourceManager::RemoveAllEffects()
{
	// �G�t�F�N�g�̔j��
	for (auto effect : mEffects)
	{
		delete effect.second;
	}
	mEffects.clear();
}

