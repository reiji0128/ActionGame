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
	/// グラフィックリソースマネージャの初期化処理
	/// </summary>
	static void Initialize();

	/// <summary>
	/// グラフィックリソースマネージャの終了処理
	/// </summary>
	static void Finalize();

	/// <summary>
	/// 使用するシェーダーの検索
	/// </summary>
	/// <param name="findShaderTag">検索するシェーダーのタグ</param>
	/// <returns>タグに対応するシェーダー</returns>
	static Shader* FindUseShader(ShaderTag findShaderTag);

	/// <summary>
	/// コンテナにあるテクスチャ・メッシュリソースの表示（デバッグ用）
	/// </summary>
	static void ShowResource();

	/// <summary>
	/// テクスチャの取得
	/// </summary>
	/// <param name="fileName">テクスチャのファイルパス</param>
	/// <returns>テクスチャクラスのポインタ</returns>
	static Texture* LoadTexture(const std::string& filePass);

	/// <summary>
	/// メッシュの取得
	/// </summary>
	/// <param name="filePass">メッシュのファイルパス</param>
	/// <returns>メッシュクラスのポインタ</returns>
	static Mesh* LoadMesh(const std::string& filePass);

	/// <summary>
	/// スケルトンの取得
	/// </summary>
	/// <param name="filePass">スケルトンファイルパス</param>
	/// <returns>スケルトンクラスのポインタ</returns>
	const static Skeleton* LoadSkeleton(const char* filePass);

	/// <summary>
	/// アニメーションの取得
	/// </summary>
	/// <param name="filePass">アニメーションのファイルパス</param>
	/// <param name="isAnimationLoop">アニメーションをループさせるかどうか</param>
	/// <returns>アニメーションクラスのポインタ</returns>
	const static Animation* LoadAnimation(const char* filePass, bool isAnimationLoop);


	static EffekseerEffect* GetEffect(const char16_t* filePass);

private:

	/// <summary>
	/// シングルトン
	/// </summary>
	GraphicResourceManager();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GraphicResourceManager();

	/// <summary>
	/// シェーダー読み込み処理
	/// </summary>
	/// <returns>
	/// true  : 読み込み成功
	/// false : 読み込み失敗
	/// </returns>
	bool LoadShaders();

	/// <summary>
	/// コンテナにシェーダーを追加
	/// </summary>
	/// <param name="addShader">追加するシェーダー</param>
	/// <param name="addShaderTag">追加するシェーダーのタグ</param>
	void AddShader(Shader* addShader, ShaderTag addShaderTag);

	/// <summary>
	/// すべてのシェーダーの削除
	/// </summary>
	void RemoveAllShader();

	/// <summary>
	/// すべてのテクスチャの削除
	/// </summary>
	void RemoveAllTexture();

	/// <summary>
	/// すべてのメッシュの削除
	/// </summary>
	void RemoveAllMeshs();

	/// <summary>
	/// すべてのスケルトンの削除
	/// </summary>
	void RemoveAllSkeletons();

	/// <summary>
	/// すべてのアニメーションの削除
	/// </summary>
	void RemoveAllAnims();

	/// <summary>
	/// すべてのエフェクトの削除
	/// </summary>
	void RemoveAllEffects();

	// マネージャの実体（アプリ内に唯一存在)
	static GraphicResourceManager* mInstance;

	// シェーダーを格納するコンテナ
	std::unordered_map<ShaderTag, Shader*> mShaders;

	// テクスチャを格納するコンテナ
	std::unordered_map<std::string, Texture*> mTextures;

	// メッシュを格納するコンテナ
	std::unordered_map<std::string, Mesh*> mMeshs;

	// スケルトンを格納するコンテナ
	std::unordered_map<std::string, Skeleton*> mSkeletons;

	// アニメーションを格納するコンテナ
	std::unordered_map<std::string, Animation*> mAnims;

	// エフェクトを格納するコンテナ
	std::unordered_map<const char16_t*, class EffekseerEffect*> mEffects;
};