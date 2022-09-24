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
class SpriteComponent;
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
	void SetAmbientLight(const Vector3& ambientColor){ mAmbientLight = ambientColor; }


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
	/// キューブマップで使用するVertexArrayのポインタの取得
	/// </summary>
	/// <returns>キューブマップで使用するVertexArrayのポインタ</returns>
	VertexArray* GetCubeMapVerts() { return mCubeMapVerts; }


	void AddMeshComponent(class MeshComponent* mesh, ShaderTag shaderTag);    // メッシュコンポーネントの追加
	void RemoveMeshComponent(class MeshComponent* mesh,ShaderTag shaderTag);  // メッシュコンポーネントの削除
	void AddSprite(SpriteComponent* Sprite);
	void RemoveSprite(SpriteComponent* sprite);
	void WindowClear() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); } // ウィンドウ描画クリア
	void WindowFlip() { SDL_GL_SwapWindow(mWindow); }                        // ウィンドウフリップ
	void SetWindowTitle(const std::string& title);                           // ウィンドウタイトルのセット

	void SpriteDrawBegin();
	void SpriteDrawEnd();

	void DrawTexture(class Texture* texture,int index, int xDivNum, int yDivNum,
		             const Vector2& offset, float scale = 1.0f, float alpha = 1.0f);
	void DrawTexture(class Texture* texture, const Vector2& offset,
		              float scale = 1.0f, float alpha = 1.0f);

	void DrawHelthGaugeTexture(class Texture* texture, int index, int xDivNum, int yDivNum,
		                       const Vector2& offset, float scaleX, float scaleY, float alpha);
	void DrawHelthGauge(class Texture* texture, const Vector2& offset,
		                                  float scaleX, float scaleY, float alpha = 1.0f);

	// Effekseer関連
	Effekseer::RefPtr<EffekseerRendererGL::Renderer> GetEffekseerRenderer() { return mEffekseerRenderer; }
	Effekseer::RefPtr<Effekseer::Manager> GetEffekseerManager() { return mEffekseerManager; }

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

	/// <summary>
	/// ライトの減衰半径の計算
	/// </summary>
	/// <param name="constant">定数</param>
	/// <param name="linear">線形</param>
	/// <param name="quadratic">2乗項</param>
	void CalcAttenuationLightRadius(const float constant, const float linear, const float quadratic);

	// スクリーンの幅
	int mScreenWidth;

	// スクリーンの高さ
	int mScreenHeight;

	std::vector<SpriteComponent*>                     mSprites;           // スプライトの描画に使われるスプライトコンポーネントのポインタの可変長コンテナ
	std::vector<class MeshComponent*>                 mMeshComponents;    // メッシュコンポーネント登録配列
	std::vector<class MeshComponent*>                 mHighLightMeshes;   // HDRメッシュ
	std::vector<class MeshComponent*>                 mNoramlMeshes;
	std::vector<class SkeletalMeshComponent*>         mSkeletalMeshes;    // スケルタルメッシュの描画に使われる
	class CubeMapComponent* mSkyBox;

// レンダラー関連 //
	
	//デプスレンダラー
	DepthMap* mDepthMapRenderer;

	// HDRレンダラー
	HDR* mHDRRenderer;

	// G-Bufferレンダラー
	GBuffer* mGBufferRenderer;

// 基本行列関連 //

	// ビュー行列
	Matrix4 mView;

	// プロジェクション行列
	Matrix4 mProjection;

	// ライト空間行列
	Matrix4 mLightSpaceMat;

// 頂点配列 //
	class VertexArray* mSpriteVerts;
	class VertexArray* mHealthVerts;
	class VertexArray* mCubeMapVerts;

	// フレームバッファID
	unsigned int mLightFBO;
	unsigned int mLightHDRTex;

// ライティング関連 //
	
    // アンビエントライト
	Vector3 mAmbientLight;

	// ディレクショナルライト
	DirectionalLight mDirectionalLight;

	// ライトの減衰半径
	std::vector<float> mLightRadius;

	std::vector<Vector3> mLightPos;
	std::vector<Vector3> mLightColor;

// レンダリングベース情報関連 //

	// SDLウィンドウハンドル
	SDL_Window* mWindow; 

	// OpenGLコンテキスト
	SDL_GLContext mContext;

	// SDLレンダリングハンドル
	SDL_Renderer* mSDLRenderer;

	const char* mGlslVersion;      // GLSLのバージョン

// Effekseer関連 //

	// Effekseerレンダラー
	Effekseer::RefPtr<EffekseerRendererGL::Renderer> mEffekseerRenderer;

	// Effekseerマネージャ
	Effekseer::RefPtr<Effekseer::Manager>            mEffekseerManager;
};

// OpenGLのエラーハンドル取得
bool GLErrorHandle(const char* location);