#include "Renderer.h"
#include "Game.h"
#include "GraphicResourceManager.h"
#include <SDL.h>
#include <SDL_image.h>
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include "Texture.h"
#include "Mesh.h"
#include "Shader.h"
#include "MeshComponent.h"
#include "Skeleton.h"
#include "SkeletalMeshComponent.h"
#include "SpriteComponent.h"
#include "Animation.h"
#include "PhysicsWorld.h"
#include "Effekseer.h"
#include "EffekseerEffect.h"
#include "DepthMap.h"
#include "HDR.h"
#include "CubeMapComponent.h"

Renderer::Renderer()
	:mWindow(nullptr)
	,mSDLRenderer(nullptr)
	,mContext(0)
{
}

Renderer::~Renderer()
{
}

bool Renderer::Initialize(int screenWidth, int screenHeight, bool fullScreen)
{
	const char* glslVersion = "#version 330";

	mScreenWidth = screenWidth;
	mScreenHeight = screenHeight;

//	OpenGLの各属性を設定 //
	// コアOpenGLプロファイルを使う
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	// GL version 3.4
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

	// 8Bit RGBA チャンネル
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE   , 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE , 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE  , 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE , 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE , 24);

	// ダブルバッファを有効
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	// ハードウェアアクセラレーションを使う
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

	// Windowの作成
	mWindow = SDL_CreateWindow("SDL & GL Window",
		                       100,
		                       80,
		                       mScreenWidth,
		                       mScreenHeight,
		                       SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

	if (!mWindow)
	{
		printf("Windowの作成に失敗: %s", SDL_GetError());
		return false;
	}
	if (fullScreen)
	{
		if (SDL_SetWindowFullscreen(mWindow, SDL_WINDOW_FULLSCREEN_DESKTOP))
		{
			printf("(%d, %d) サイズのフルスクリーン化に失敗\n", screenWidth, screenHeight);
			return false;
		}
		mScreenWidth = 1920;
		mScreenHeight = 1080;
		glViewport(0, 0, mScreenWidth, mScreenHeight);
	}

	//SDLRendererの作成
	mSDLRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!mSDLRenderer)
	{
		printf("SDLRendererの作成に失敗 : %s", SDL_GetError());
		return false;
	}
	if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
	{
		printf("SDLImageInitPNGの初期化に失敗 : %s", SDL_GetError());
		return false;
	}

	// OpenGLContextの作成
	mContext = SDL_GL_CreateContext(mWindow);

	// Glewの初期化
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		printf("GLEWの初期化に失敗");
		return false;
	}

	// 幾つかのプラットホームでは、GLEWが無害なエラーコードを吐くのでクリアしておく
	glGetError();

	// デプスレンダラーの初期化
	mDepthMapRenderer = new DepthMap;
	mDepthMapRenderer->CreateShadowMap();

	// HDRの初期化
	mHDRRenderer = new HDR;
	mHDRRenderer->CreateHDRBuffer();

	// カリング
	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);
	
	// スプライト用の頂点配列を作成
	CreateSpriteVerts();
	// 体力ゲージ用の頂点配列を作成
	CreateHealthGaugeVerts();
	// キューブマップ用の頂点配列を作成
	CreateCubeMapVerts();

	// スクリーン全体を覆う頂点バッファオブジェクトを作成
	unsigned int screenVAO;
	ScreenVAOSetting(screenVAO);

	// Effekseer初期化
	mEffekseerRenderer = ::EffekseerRendererGL::Renderer::Create(8000, EffekseerRendererGL::OpenGLDeviceType::OpenGL3);
	mEffekseerManager = ::Effekseer::Manager::Create(8000);

	// 描画モジュール作成
	mEffekseerManager->SetSpriteRenderer(mEffekseerRenderer->CreateSpriteRenderer());
	mEffekseerManager->SetRibbonRenderer(mEffekseerRenderer->CreateRibbonRenderer());
	mEffekseerManager->SetRingRenderer(mEffekseerRenderer->CreateRingRenderer());
	mEffekseerManager->SetTrackRenderer(mEffekseerRenderer->CreateTrackRenderer());
	mEffekseerManager->SetModelRenderer(mEffekseerRenderer->CreateModelRenderer());

	// Effekseer用のテクスチャ・モデル・マテリアルローダー
	mEffekseerManager->SetTextureLoader(mEffekseerRenderer->CreateTextureLoader());
	mEffekseerManager->SetModelLoader(mEffekseerRenderer->CreateModelLoader());
	mEffekseerManager->SetMaterialLoader(mEffekseerRenderer->CreateMaterialLoader());

	return true;
}

void Renderer::Shutdown()
{
	// Effekseer関連の破棄
	mEffekseerManager.Reset();
	mEffekseerRenderer.Reset();

	// SDL系の破棄
	SDL_GL_DeleteContext(mContext);
	SDL_DestroyWindow(mWindow);
}

void Renderer::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);


	// ライト空間行列を取得
	Matrix4 lightSpaceMat = mDepthMapRenderer->GetLightSpaceMatrix();

	// 深度バッファへの焼きこみ処理
	BakeDepthBuffer(lightSpaceMat);

	/*mNormalShader->SetActive();
	mNormalShader->SetVectorUniform("in_light.position", mDirectionalLight.mDirection);
	mNormalShader->SetVectorUniform("in_light.diffuse", mDirectionalLight.mDiffuseColor);
	mNormalShader->SetVectorUniform("in_light.specular", mDirectionalLight.mSpecColor);
	mNormalShader->SetVectorUniform("in_light.ambient", mAmbientLight);
	mNormalShader->SetMatrixUniform("view", mView);
	mNormalShader->SetMatrixUniform("projection", mProjection);
	mNormalShader->SetVectorUniform("viewPos", GAMEINSTANCE.GetViewPos());
	mNormalShader->SetIntUniform("normalMap", 1);
	for (auto sk : mNoramlMeshes)
	{
		if (sk->GetVisible())
		{
			sk->Draw(mNormalShader);
		}
	}*/

	mHDRRenderer->HDRRenderingBegin();
	{
		Shader* useShader = nullptr;

		// スカイボックスがなければスカイボックスを描画
		if (mSkyBox != nullptr)
		{
			useShader = GraphicResourceManager::FindUseShader(ShaderTag::SKYBOX);
			useShader->SetActive();
			// ゲームの空間に合わせるためのオフセット行列をセット
			Matrix4 offset = Matrix4::CreateRotationX(Math::ToRadians(90.0f));

			// Uniformに逆行列をセット
			Matrix4 InvView = mView;
			InvView.Invert();
			InvView.Transpose();
			useShader->SetMatrixUniform("uOffset"     , offset);
			useShader->SetMatrixUniform("uProjection" , mProjection);
			useShader->SetMatrixUniform("uView"       , InvView);
			useShader->SetIntUniform("uSkyBox"        , 0);

			mSkyBox->Draw(useShader);
		}

		// テクスチャスロット1番をアクティブにしてデプスマップをバインド
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, mDepthMapRenderer->GetDepthTexID());
		
	// メッシュ用のシェーダー //
		useShader = GraphicResourceManager::FindUseShader(ShaderTag::SHADOW_MAP);

		useShader->SetActive();
		// Uniform変数にライトをセット
		SetLightUniforms(useShader);
		useShader->SetIntUniform("depthMap"             , 1);
		useShader->SetMatrixUniform("uView"             , mView);
		useShader->SetMatrixUniform("uProjection"       , mProjection);
		useShader->SetMatrixUniform("uLightSpaceMatrix" , lightSpaceMat);

		// メッシュの描画
		for (auto mc : mMeshComponents)
		{
			if (mc->GetVisible())
			{
				mc->Draw(useShader);
			}
		}

	// スキンメッシュ用のシャドウマップ //
		useShader = GraphicResourceManager::FindUseShader(ShaderTag::SKINNED_SHADOW_MAP);
		useShader->SetActive();
		// Uniform変数にライトをセット
		SetLightUniforms(useShader);
		// デプスマップのセット
		useShader->SetIntUniform("depthMap"                   , 1);
		// ビュー射影行列のセット
		useShader->SetMatrixUniform("uView"                   , mView);
		useShader->SetMatrixUniform("uProjection"             , mProjection);
		useShader->SetMatrixUniform("uViewProj"               , mView * mProjection);
		// ライト空間行列のセット
		useShader->SetMatrixUniform("uLightSpaceMatrix"       , lightSpaceMat);

		// スケルタルメッシュの描画
		for (auto sk : mSkeletalMeshes)
		{
			if (sk->GetVisible())
			{
				sk->Draw(useShader);
			}
		}

		////球体の描画
		//Vector3 lightColor(0.8, 0.5, 0.2);
		//mHDRShader->SetActive();
		//mHDRShader->SetMatrixUniform("uViewProj", mView * mProjection);
		//mHDRShader->SetVectorUniform("color", lightColor);
		//mHDRShader->SetFloatUniform("luminance", 10.0f);
		//// 全てのメッシュコンポーネントを描画
		//for (auto mc : mHighLightMeshes)
		//{
		//	if (mc->GetVisible())
		//	{
		//		mc->Draw(mHDRShader);
		//	}
		//}
	}
	mHDRRenderer->HDRRenderingEnd();

	mHDRRenderer->ScaleDownBufferPath();
	mHDRRenderer->HDRBloomBlend();

	GAMEINSTANCE.GetPhysics()->DebugShowBox();
}

void Renderer::SetViewMatrix(const Matrix4& view)
{
	Matrix4 tmp = view;
	mView = view;

	// Effekseer に座標系を合わせて行列をセットする
	Effekseer::Matrix44 efCam;
	tmp.mat[0][0] *= -1;
	tmp.mat[0][1] *= -1;
	tmp.mat[1][2] *= -1;
	tmp.mat[2][2] *= -1;
	tmp.mat[3][2] *= -1;

	efCam = tmp;
	RENDERER->GetEffekseerRenderer()->SetCameraMatrix(efCam);
}

void Renderer::SetProjMatrix(const Matrix4& proj)
{
	mProjection = proj;

	// Effekseer に座標系を合わせて行列をセットする
	Matrix4 tmp = proj;
	tmp.mat[2][2] *= -1;
	tmp.mat[2][3] *= -1;

	Effekseer::Matrix44 eProj;
	eProj = tmp;
	RENDERER->GetEffekseerRenderer()->SetProjectionMatrix(eProj);
}

void Renderer::SetDepthSetting(const Vector3& centerPos, const Vector3& lightDir, const Vector3& upVec, const float lightDistance)
{
	mDepthMapRenderer->CalcLightSpaceMatrix(centerPos, lightDir, upVec, lightDistance);
}

void Renderer::AddMeshComponent(MeshComponent* mesh, ShaderTag shaderTag)
{

	if(shaderTag == ShaderTag::SKINNED_SHADOW_MAP)
	{
		SkeletalMeshComponent* sk = static_cast<SkeletalMeshComponent*>(mesh);
		mSkeletalMeshes.emplace_back(sk);
	}
	else if(shaderTag == ShaderTag::SHADOW_MAP)
	{
		mMeshComponents.emplace_back(mesh);
	}
	else if (shaderTag == ShaderTag::HDR)
	{
		mHighLightMeshes.emplace_back(mesh);
	}
	else if (shaderTag == ShaderTag::NORMAL_MAP)
	{
		mNoramlMeshes.emplace_back(mesh);
	}
}

void Renderer::RemoveMeshComponent(MeshComponent* mesh,ShaderTag shaderTag)
{
	if (shaderTag == ShaderTag::SKINNED_SHADOW_MAP)

	{
		SkeletalMeshComponent* sk = static_cast<SkeletalMeshComponent*>(mesh);
		auto iter = std::find(mSkeletalMeshes.begin(), mSkeletalMeshes.end(), sk);
		mSkeletalMeshes.erase(iter);
	}
	else if (shaderTag == ShaderTag::SHADOW_MAP)
	{
		auto iter = std::find(mMeshComponents.begin(), mMeshComponents.end(), mesh);
		mMeshComponents.erase(iter);
	}
	else if (shaderTag == ShaderTag::HDR)
	{
		auto iter = std::find(mHighLightMeshes.begin(), mHighLightMeshes.end(), mesh);
		mHighLightMeshes.erase(iter);
	}
	else if (shaderTag == ShaderTag::NORMAL_MAP)
	{
		auto iter = std::find(mNoramlMeshes.begin(), mNoramlMeshes.end(), mesh);
		mNoramlMeshes.erase(iter);
	}

}

/// <summary>
/// スプライトの追加
/// </summary>
/// <param name="sprite">追加するSpriteComponentクラスのポインタ</param>
void Renderer::AddSprite(SpriteComponent* sprite)
{
	// ソート済みの配列で挿入点を見つける
	// (DrawOrderが小さい順番に描画するため)
	int myDrowOrder = sprite->GetDrawOrder();
	auto iter = mSprites.begin();

	for (; iter != mSprites.end(); ++iter)
	{
		if (myDrowOrder < (*iter)->GetDrawOrder())
		{
			break;
		}
	}

	// イテレーターの位置の前に要素を挿入する
	mSprites.insert(iter, sprite);
}

/// <summary>
/// スプライトの削除
/// </summary>
/// <param name="sprite">削除するSpriteComponentクラスのポインタ</param>
void Renderer::RemoveSprite(SpriteComponent* sprite)
{
	auto iter = std::find(mSprites.begin(), mSprites.end(), sprite);
	mSprites.erase(iter);
}

void Renderer::CreateSpriteVerts()
{
	float vertices[] = 
	{
	    //    位置　　　 |　   法線       |  uv座標
		-0.5f, 0.5f, 0.f,  0.f, 0.f, 0.0f,  0.f, 0.f, // 左上
		 0.5f, 0.5f, 0.f,  0.f, 0.f, 0.0f,  1.f, 0.f, // 右上
		 0.5f,-0.5f, 0.f,  0.f, 0.f, 0.0f,  1.f, 1.f, // 右下
		-0.5f,-0.5f, 0.f,  0.f, 0.f, 0.0f,  0.f, 1.f  // 左下
	};

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	mSpriteVerts = new VertexArray(vertices, 4, VertexArray::PosNormTex, indices, 6);
}

void Renderer::CreateHealthGaugeVerts()
{
	float vertices[] =
	{
	    //    位置　　　 |　   法線       |  uv座標
		0.0f, 1.0f, 0.0f,  0.f, 0.f, 0.0f,  0.f, 0.f,	//左上  0
		1.0f, 1.0f, 0.0f,  0.f, 0.f, 0.0f,  1.f, 0.f,	//右上  1
		0.0f, 0.0f, 0.0f,  0.f, 0.f, 0.0f,  1.f, 1.f,	//左下  2
		1.0f, 0.0f, 0.0f,  0.f, 0.f, 0.0f,  0.f, 1.f,	//右下  3
	};

	unsigned int indices[] =
	{
		0, 1, 2,
		2, 3, 1
	};

	mHealthVerts = new VertexArray(vertices, 4, VertexArray::PosNormTex, indices, 6);
}

void Renderer::CreateCubeMapVerts()
{
	mCubeMapVerts = new VertexArray();
	mCubeMapVerts->CreateCubeMapVAO();
}

void Renderer::ScreenVAOSetting(unsigned int& vao)
{
	unsigned int vbo;
	float quadVertices[] =
	{
		//      位置       |   uv座標
		-1.0f,  1.0f, 0.0f,  0.0f, 0.0f,   // 左上
		-1.0f, -1.0f, 0.0f,  0.0f, 0.0f,   // 左下
		 1.0f,  1.0f, 0.0f,  1.0f, 1.0f,   // 右上
		 1.0f, -1.0f, 0.0f,  1.0f, 0.0f    // 右下
	};

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
}

void Renderer::BakeDepthBuffer(Matrix4 lightSpaceMatrix)
{
	// デプスレンダリングパス開始
	mDepthMapRenderer->DepthRenderingBegin();
	{
		Shader* useShader = nullptr;

		// メッシュをデプスマップにレンダリング
		useShader = GraphicResourceManager::FindUseShader(ShaderTag::DEPTH_MAP);
		useShader->SetActive();
		useShader->SetMatrixUniform("lightSpaceMatrix", lightSpaceMatrix);
		for (auto mc : mMeshComponents)
		{
			if (mc->GetVisible())
			{
				mc->Draw(useShader);
			}
		}

		// スキンメッシュをデプスマップにレンダリング
		useShader = GraphicResourceManager::FindUseShader(ShaderTag::SKINNED_DEPTH_MAP);
		useShader->SetActive();
		useShader->SetMatrixUniform("uLightSpaceMat", lightSpaceMatrix);
		for (auto sk : mSkeletalMeshes)
		{
			if (sk->GetVisible())
			{
				sk->Draw(useShader);
			}
		}
	}
	// デプスレンダリングの終了
	mDepthMapRenderer->DepthRenderingEnd();
}

void Renderer::SetWindowTitle(const std::string& title)
{
	SDL_SetWindowTitle(mWindow, title.c_str());
}

void Renderer::SpriteDrawBegin()
{
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

	mSpriteVerts->SetActive();
}

void Renderer::SpriteDrawEnd()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glDisable(GL_BLEND);
}

// テクスチャの描画
void Renderer::DrawTexture(class Texture* texture, int index, int xDivNum, int yDivNum, const Vector2& offset, float scale, float alpha)
{
	Shader* useShader = nullptr;
	useShader = GraphicResourceManager::FindUseShader(ShaderTag::TILEMAP);

	// テクスチャの幅・高さでスケーリング
	Matrix4 scaleMat = Matrix4::CreateScale(static_cast<float>(texture->GetWidth() / xDivNum) * scale,
		                                    static_cast<float>(texture->GetHeight() / yDivNum) * scale,
		                                    1.0f);
	// スクリーン位置の平行移動
	Matrix4 transMat = Matrix4::CreateTranslation(Vector3(offset.x - (mScreenWidth * 0.5f),
		                                          (mScreenHeight * 0.5f) - offset.y, 0.0f));
	// ワールド変換
	Vector2 tileSplitNum(static_cast<float>(xDivNum), static_cast<float>(yDivNum));
	Matrix4 world = scaleMat * transMat;

	useShader->SetActive();
	useShader->SetVector2Uniform("uTileSetSplitNum", tileSplitNum);
	useShader->SetMatrixUniform("uWorldTransform", world);
	useShader->SetFloatUniform("uAlpha", alpha);
	useShader->SetIntUniform("uTileIndex", index);
	// テクスチャセット
	texture->SetActive();

	// 四角形描画
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

void Renderer::DrawTexture(Texture* texture, const Vector2& offset, float scale, float alpha)
{
	DrawTexture(texture, 0, 1, 1, offset, scale, alpha);
}

// 体力ゲージの描画
void Renderer::DrawHelthGaugeTexture(Texture* texture, int index, int xDivNum, int yDivNum, const Vector2& offset, float scaleX, float scaleY, float alpha)
{
	Shader* useShader = nullptr;
	useShader = GraphicResourceManager::FindUseShader(ShaderTag::TILEMAP);

	mHealthVerts->SetActive();
	// テクスチャの幅・高さでスケーリング
	Matrix4 scaleMat = Matrix4::CreateScale(static_cast<float>(texture->GetWidth() / xDivNum) * scaleX,
		static_cast<float>(texture->GetHeight() / yDivNum) * scaleY,
		1.0f);

	// スクリーン位置の平行移動
	Matrix4 transMat = Matrix4::CreateTranslation(Vector3(offset.x - (mScreenWidth * 0.5f),
		(mScreenHeight * 0.5f) - offset.y,
		0.0f));
	// ワールド変換
	Vector2 tileSplitNum(static_cast<float>(xDivNum), static_cast<float>(yDivNum));
	Matrix4 world = scaleMat * transMat;
	useShader->SetActive();
	useShader->SetMatrixUniform("uWorldTransform", world);
	useShader->SetIntUniform("uTileIndex", index);
	useShader->SetVector2Uniform("uTileSetSplitNum", tileSplitNum);
	useShader->SetFloatUniform("uAlpha", alpha);
	// テクスチャセット
	texture->SetActive();

	// 四角形描画
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}


void Renderer::DrawHelthGauge(Texture* texture, const Vector2& offset, float scaleX, float scaleY, float alpha)
{
	DrawHelthGaugeTexture(texture, 0, 1, 1, offset, scaleX, scaleY, alpha);
}


void Renderer::SetLightUniforms(Shader* shader)
{
	// ビュー行列からカメラ位置を逆算出する
	Matrix4 invView = mView;
	invView.Invert();
	shader->SetVectorUniform("uCameraPos", invView.GetTranslation());

	//アンビエントライト
	shader->SetVectorUniform("uAmbientLight", mAmbientLight);

	//ディレクショナルライト
	shader->SetVectorUniform("uDirLight.mDirection", mDirectionalLight.mDirection);
	shader->SetVectorUniform("uDirLight.mDiffuseColor", mDirectionalLight.mDiffuseColor);
	shader->SetVectorUniform("uDirLight.mSpecColor", mDirectionalLight.mSpecColor);
}

bool GLErrorHandle(const char* location)
{
	GLenum error_code = glGetError();
	if (error_code == GL_NO_ERROR)
	{
		return true;
	}
	do
	{
		const char* msg;
		switch (error_code)
		{
		case GL_INVALID_ENUM:                  msg = "INVALID_ENUM"; break;
		case GL_INVALID_VALUE:                 msg = "INVALID_VALUE"; break;
		case GL_INVALID_OPERATION:             msg = "INVALID_OPERATION"; break;
		case GL_INVALID_FRAMEBUFFER_OPERATION: msg = "INVALID_FRAMEBUFFER_OPERATION"; break;
		default: msg = "unknown Error";
		}
		printf("GLErrorLayer: ERROR%04x'%s' location: %s\n", error_code, msg, location);
		error_code = glGetError();
	} while (error_code != GL_NO_ERROR);

	return false;
}