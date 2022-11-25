#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <SDL.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <Effekseer.h>
#include <EffekseerRendererGL.h>

#include "Math.h"
#include "VertexArray.h"
#include "ShaderTag.h"

// ディレクショナルライト構造体
typedef struct DirectionalLight
{
	//ライトの方向
	Vector3 mDirection;
	//拡散反射の色
	Vector3 mDiffuseColor;
	//鏡面反射の色
	Vector3 mSpecColor;

}DirectionalLight;

// 前方宣言
class Mesh;
class MeshComponent;
class SkeletalMeshComponent;
class PointLightComponent;
class SpriteComponent;
class ImageComponent;
class CubeMapComponent;
class DepthMap;
class HDR;
class GBuffer;

class Renderer
{
public:
	Renderer();

	~Renderer();

	/// <summary>
	/// SDL と GLの初期化
	/// </summary>
	/// <param name="screenWidth">スクリーンの幅</param>
	/// <param name="screenHeight">スクリーンの高さ</param>
	/// <param name="fullScreen">
	/// true  : フルスクリーン
	/// false : ウィンドウ
	/// </param>
	/// <returns>
	/// true  : 初期化成功
	/// false : 初期化失敗
	/// </returns>
	bool Initialize(int screenWidth, int screenHeight, bool fullScreen);

	/// <summary>
	/// シャットダウン処理
	/// </summary>
	void Shutdown();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	// セッター //

		/// <summary>
		/// ビュー行列のセット
		/// </summary>
		/// <param name="view">ビュー行列</param>
	void SetViewMatrix(const Matrix4& view);

	/// <summary>
	/// プロジェクション行列のセット
	/// </summary>
	/// <param name="proj">プロジェクション行列</param>
	void SetProjMatrix(const Matrix4& proj);

	/// <summary>
	/// アンビエントライトのセット
	/// </summary>
	/// <param name="ambientColor">アンビエント色</param>
	void SetAmbientLight(const Vector3& ambientColor) { mAmbientLight = ambientColor; }


	/// <summary>
	/// デプスマップで使う値のセット
	/// </summary>
	/// <param name="centerPos">ステージの中心座標</param>
	/// <param name="lightDir">ライトの向き</param>
	/// <param name="upVec">上向きのベクトル</param>
	/// <param name="lightDistance">ライトの距離</param>
	void SetDepthSetting(const Vector3& centerPos, const Vector3& lightDir, const Vector3& upVec, const float lightDistance);

	/// <summary>
	/// スカイボックスをアクティブにセット
	/// </summary>
	/// <param name="cubeMapComp">キューブマップコンポーネントのポインタ</param>
	void SetActiveSkyBox(class CubeMapComponent* cubeMapComp) { mSkyBox = cubeMapComp; }

	// ゲッター //

		/// <summary>
		/// SDLレンダラーの取得
		/// </summary>
		/// <returns>SDLレンダラークラスのポインタ</returns>
	SDL_Renderer* GetSDLRenderer() { return mSDLRenderer; }

	/// <summary>
	/// スクリーンの幅の取得
	/// </summary>
	/// <returns>スクリーンの幅</returns>
	float GetScreenWidth() { return static_cast<float>(mScreenWidth); }

	/// <summary>
	/// スクリーンの高さ
	/// </summary>
	/// <returns>スクリーンの高さ</returns>
	float GetScreenHeight() { return static_cast<float>(mScreenHeight); }

	/// <summary>
	/// ディレクショナルライトの取得
	/// </summary>
	/// <returns>ディレクショナルライト取得</returns>
	DirectionalLight& GetDirectionalLight() { return mDirectionalLight; }

	/// <summary>
	/// ビュー行列の取得
	/// </summary>
	/// <returns>ビュー行列の取得</returns>
	const Matrix4& GetViewMatrix() { return mView; }

	/// <summary>
	/// プロジェクション行列の取得
	/// </summary>
	/// <returns>プロジェクション行列の取得</returns>
	const Matrix4& GetProjectionMatrix() { return mProjection; }

	/// <summary>
	/// ビュープロジェクション行列の取得
	/// </summary>
	/// <returns></returns>
	const Matrix4& GetSimpleViewProjMatrix() { return mSimpleViewProjMat; }

	/// <summary>
	/// キューブマップで使用するVertexArrayのポインタの取得
	/// </summary>
	/// <returns>キューブマップで使用するVertexArrayのポインタ</returns>
	VertexArray* GetCubeMapVerts() { return mCubeMapVerts; }

	/// <summary>
	/// エフェクシアレンダラーの取得
	/// </summary>
	/// <returns>エフェクシアレンダラー</returns>
	Effekseer::RefPtr<EffekseerRendererGL::Renderer> GetEffekseerRenderer() { return mEffekseerRenderer; }

	/// <summary>
	/// エフェクシアマネージャーの取得
	/// </summary>
	/// <returns>エフェクシアマネージャー</returns>
	Effekseer::RefPtr<Effekseer::Manager> GetEffekseerManager();
	/// <summary>
	/// メッシュコンポーネントの追加
	/// </summary>
	/// <param name="mesh">追加するメッシュコンポーネント</param>
	/// <param name="shaderTag">シェーダーのタグ</param>
	void AddMeshComponent(MeshComponent* mesh, ShaderTag shaderTag);

	/// <summary>
	/// メッシュコンポーネントの削除
	/// </summary>
	/// <param name="mesh">削除するメッシュコンポーネント</param>
	/// <param name="shaderTag">シェーダーのタグ</param>
	void RemoveMeshComponent(MeshComponent* mesh, ShaderTag shaderTag);

	/// <summary>
	/// スプライトの追加
	/// </summary>
	/// <param name="sprite">追加するスプライトコンポーネント</param>
	void AddSprite(SpriteComponent* sprite);

	/// <summary>
	/// スプライトの削除
	/// </summary>
	/// <param name="sprite">削除するスプライトコンポーネント</param>
	void RemoveSprite(SpriteComponent* sprite);

	/// <summary>
	/// ポイントライトの追加
	/// </summary>
	/// <param name="pointLight">追加するポイントライトコンポーネント</param>
	void AddPointLight(PointLightComponent* pointLight);

	/// <summary>
	/// ポイントライトの削除
	/// </summary>
	/// <param name="pointLight">削除するポイントライトコンポーネント</param>
	void RemovePointLight(PointLightComponent* pointLight);

	/// <summary>
	/// ウィンドウの描画クリア
	/// </summary>
	void WindowClear() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

	/// <summary>
	/// ウィンドウフリップ
	/// </summary>
	void WindowFlip() { SDL_GL_SwapWindow(mWindow); }

	/// <summary>
	/// ウィンドウタイトルのセット
	/// </summary>
	/// <param name="title"></param>
	void SetWindowTitle(const std::string& title);

	/// <summary>
	/// スプライトの描画開始処理
	/// </summary>
	void SpriteDrawBegin();

	/// <summary>
	/// スプライトの描画終了処理
	/// </summary>
	void SpriteDrawEnd();

	void DrawTexture(class Texture* texture, int index, int xDivNum, int yDivNum,
		const Vector2& offset, float scale = 1.0f, float alpha = 1.0f);
	void DrawTexture(class Texture* texture, const Vector2& offset,
		float scale = 1.0f, float alpha = 1.0f);

	/// <summary>
	/// スプライトの頂点配列をアクティブに変更
	/// </summary>
	void ChangeActievSpriteVertex();

	/// <summary>
	/// HPゲージの頂点配列をアクティブに変更
	/// </summary>
	void ChangeActiveHPGaugeVertex();

private:

	/// <summary>
	/// ライトの値をシェーダーにセット
	/// </summary>
	/// <param name="shader">使用するシェーダー</param>
	void SetLightUniforms(class Shader* shader);

	/// <summary>
	/// スプライトの頂点配列作成
	/// </summary>
	void CreateSpriteVerts();

	/// <summary>
	/// 体力ゲージ用の頂点作成
	/// </summary>
	void CreateHealthGaugeVerts();

	/// <summary>
	/// キューブマップの頂点作成
	/// </summary>
	void CreateCubeMapVerts();

	// ライト用のフレームバッファオブジェクト作成
	void CreateLightFBO();

	/// <summary>
	/// 画面全体を覆う頂点定義
	/// </summary>
	/// <param name="vao"></param>
	void ScreenVAOSetting(unsigned int& vao);

	/// <summary>
	/// スクリーンいっぱい四角形の描画
	/// </summary>
	void RenderQuad();

	/// <summary>
	/// 深度バッファへの焼きこみ処理
	/// </summary>
	void BakeDepthBuffer();

	/// <summary>
	/// G-BUfferに描画
	/// </summary>
	void DrawToGBuffer();

	/// <summary>
	/// ポイントライトパス
	/// </summary>
	void PointLightPass();

	/// <summary>
	/// ディレクショナルライトパス
	/// </summary>
	void DirectionalLightPass();

	// スクリーンの幅
	int mScreenWidth;

	// スクリーンの高さ
	int mScreenHeight;

	// スプライトコンポーネントのコンテナ
	std::vector<SpriteComponent*> mSprites;

	// メッシュコンポーネントのコンテナ
	std::vector<MeshComponent*> mMeshComponents;

	// 法線マップを適用するメッシュのコンテナ
	std::vector<MeshComponent*> mNoramlMeshes;

	// HDRを適用するメッシュのコンテナ
	std::vector <MeshComponent* > mHighLightMesh;

	// スケルタルメッシュのコンテナ
	std::vector<SkeletalMeshComponent*> mSkeletalMeshes;
	
	// ポイントライトのコンテナ
	std::vector<PointLightComponent*> mPointLights;

// レンダラー関連 //
	
	//デプスレンダラー
	DepthMap* mDepthMapRenderer;

	// HDRレンダラー
	HDR* mHDRRenderer;

	// G-Bufferレンダラー
	GBuffer* mGBufferRenderer;

	CubeMapComponent* mSkyBox;

// 基本行列関連 //

	// ビュー行列
	Matrix4 mView;

	// プロジェクション行列
	Matrix4 mProjection;

	// 正射影行列のプロジェクション行列
	Matrix4 mSimpleViewProjMat;

	// ライト空間行列
	Matrix4 mLightSpaceMat;

// 頂点配列 //
	class VertexArray* mSpriteVerts;
	class VertexArray* mHPGaugeVerts;
	class VertexArray* mCubeMapVerts;

	// フレームバッファID
	unsigned int mLightFBO;
	unsigned int mLightHDRTex;

// ライティング関連 //
	
	Mesh* mPointLightMesh;

    // アンビエントライト
	Vector3 mAmbientLight;

	// ディレクショナルライト
	DirectionalLight mDirectionalLight;

// レンダリングベース情報関連 //

	// SDLウィンドウハンドル
	SDL_Window* mWindow; 

	// OpenGLコンテキスト
	SDL_GLContext mContext;

	// SDLレンダリングハンドル
	SDL_Renderer* mSDLRenderer;

	// GLSLのバージョン
	const char* mGlslVersion;

// Effekseer関連 //

	// Effekseerレンダラー
	Effekseer::RefPtr<EffekseerRendererGL::Renderer> mEffekseerRenderer;

	// Effekseerマネージャ
	Effekseer::RefPtr<Effekseer::Manager>            mEffekseerManager;
};

// OpenGLのエラーハンドル取得
bool GLErrorHandle(const char* location);