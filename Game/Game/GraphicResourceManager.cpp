#include "GraphicResourceManager.h"
#include "Shader.h"
#include "Texture.h"
#include "Mesh.h"
#include "Skeleton.h"
#include "Animation.h"
#include "EffekseerEffect.h"

// GraphicResource実体へのポインタ定義
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

	// コンテナの中に検索しているシェーダーのタグがあれば対応するシェーダーを返す
	// 見つからなければ nullptr を返す
	if (shader != mInstance->mShaders.end())
	{
		return mInstance->mShaders[findShaderTag];
	}
	else
	{
		printf("シェーダーが見つかりませんでした\n");
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

	// コンテナの中にそのテクスチャがある場合そのテクスチャを返す
	// なければコンテナに追加してテクスチャを返す
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

	// コンテナの中にそのメッシュがある場合そのメッシュを返す
	// なければコンテナに追加してメッシュを返す
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

	// コンテナの中にそのスケルトンがある場合そのスケルトンを返す
	// なければコンテナにスケルトンを追加してを返す
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

	// コンテナの中にそのアニメーションがある場合そのアニメーションを返す
	// なければコンテナにアニメーションを追加してを返す
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

	// コンテナの中にそのエフェクトがある場合そのエフェクトを返す
	// なければコンテナにエフェクトを追加してを返す
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

	// スプライトシェーダーの読み込み
	shader = new Shader();
	if (!shader->Load("Shaders/Sprite.vert", "Shaders/Sprite.frag"))
	{
		printf("スプライトシェーダーの読み込み失敗\n");
		return false;
	}
	AddShader(shader, ShaderTag::SPRITE);


	// タイルマップシェーダーの読み込み
	shader = new Shader();
	if (!shader->Load("Shaders/Sprite.vert", "Shaders/Tilemap.frag"))
	{
		printf("タイルマップシェーダーの読み込み失敗\n");
		return false;
	}
	AddShader(shader, ShaderTag::TILEMAP);


	// メッシュ用デプスシェーダーの読み込み
	shader = new Shader();
	if (!shader->Load("Shaders/DepthMap.vert", "Shaders/DepthMap.frag"))
	{
		printf("メッシュのデプスシェーダーの読み込み失敗\n");
		return false;
	}
	AddShader(shader, ShaderTag::DEPTH_MAP);


	// スキンメッシュ用デプスシェーダーの読み込み
	shader = new Shader();
	if (!shader->Load("Shaders/SkinnedDepthMap.vert", "Shaders/DepthMap.frag"))
	{
		printf("スキンメッシュのデプスシェーダーの読み込み失敗\n");
		return false;
	}
	AddShader(shader, ShaderTag::SKINNED_DEPTH_MAP);


	// メッシュ用シャドウマップシェーダーの読み込み
	shader = new Shader();
	if (!shader->Load("Shaders/Shadowmap.vert", "Shaders/Shadowmap.frag"))
	{
		printf("シャドウマップシェーダーの読み込み失敗\n");
		return false;
	}
	AddShader(shader, ShaderTag::SHADOW_MAP);


	// スキンメッシュシェーダーの読み込み
	shader = new Shader();
	if (!shader->Load("Shaders/SkinnedShadowmap.vert", "Shaders/Shadowmap.frag"))
	{
		printf("スキンメッシュシェーダの読み込み失敗\n");
		return false;
	}
	AddShader(shader, ShaderTag::SKINNED_SHADOW_MAP);


	// HDRシェーダーの読み込み
	shader = new Shader();
	if (!shader->Load("Shaders/HighLightMesh.vert", "Shaders/HighLightMesh.frag"))
	{
		printf("スフィアシェーダの読み込み失敗\n");
		return false;
	}
	AddShader(shader, ShaderTag::HDR);

	// ダウンサンプリングシェーダーの読み込み
	shader = new Shader();
	if (!shader->Load("Shaders/ScreenBuffer.vert", "Shaders/DownSampling.frag"))
	{
		printf("ダウンサンプリングシェーダーの読み込み失敗\n");
	}
	AddShader(shader, ShaderTag::DOWNSAMPLING);


	// ガウスブラーシェーダーの読み込み
	shader = new Shader();
	if (!shader->Load("Shaders/ScreenBuffer.vert", "Shaders/GaussBlur.frag"))
	{
		printf("ガウスブラーシェーダーの読み込み失敗\n");
	}
	AddShader(shader, ShaderTag::GAUSS_BLUR);


	// HDRBloomブレンドシェーダーの読み込み
	shader = new Shader();
	if (!shader->Load("Shaders/ScreenBuffer.vert", "Shaders/HDRBloomBlend.frag"))
	{
		printf("HDRBloomブレンドシェーダーの読み込み失敗\n");
	}
	AddShader(shader, ShaderTag::HDR_BLOOM_BLEND);


	// スクリーンバッファーシェーダーの読み込み
	shader = new Shader();
	if (!shader->Load("Shaders/ScreenBuffer.vert", "Shaders/HDR.frag"))
	{
		printf("スクリーンバッファーシェーダーの読み込み失敗\n");
	}
	AddShader(shader, ShaderTag::SCREEN_BUFFER);


	// 法線マップシェーダーの読み込み
	shader = new Shader();
	if (!shader->Load("Shaders/Normal.vert", "Shaders/Normal.frag"))
	{
		printf("法線マップシェーダの読み込み失敗\n");
		return false;
	}
	AddShader(shader, ShaderTag::NORMAL_MAP);


	// スカイボックスシェーダーの読み込み
	shader = new Shader();
	if (!shader->Load("Shaders/SkyBox.vert", "Shaders/SkyBox.frag"))
	{
		printf("スカイボックスシェーダーの読み込み失敗\n");
		return false;
	}
	AddShader(shader, ShaderTag::SKYBOX);

	return true;
}

void GraphicResourceManager::AddShader(Shader* addShader, ShaderTag addShaderTag)
{
	auto shader = mShaders.find(addShaderTag);

	// コンテナの中に追加するシェーダーがすでにあれば追加しない
	// シェーダーがなければコンテナに追加
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
	//テクスチャ破棄
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
	// メッシュ破棄
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
	// スケルトンの破棄
	for (auto skeleton : mSkeletons)
	{
		printf("Skeleton Release : %s\n", skeleton.first.c_str());
		delete skeleton.second;
	}
	mSkeletons.clear();
}

void GraphicResourceManager::RemoveAllAnims()
{
	// アニメーションの破棄
	for (auto anim : mAnims)
	{
		printf("Animation Release : %s\n", anim.first.c_str());
		delete anim.second;
	}
	mAnims.clear();
}

void GraphicResourceManager::RemoveAllEffects()
{
	// エフェクトの破棄
	for (auto effect : mEffects)
	{
		delete effect.second;
	}
	mEffects.clear();
}

