#include "Renderer.h"
#include <iostream>
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
#include "GBuffer.h"
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

	// デプスレンダラーの作成
	mDepthMapRenderer = new DepthMap;
	mDepthMapRenderer->CreateShadowMap();

	// HDRバッファーの作成
	mHDRRenderer = new HDR;
	mHDRRenderer->CreateHDRBuffer();

	// G-Bufferの作成
	mGBufferRenderer = new GBuffer;
	mGBufferRenderer->CreateGBuffer();

	// カリング
	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);
	
	// スプライト用の頂点配列を作成
	CreateSpriteVerts();

	// 体力ゲージ用の頂点配列を作成
	CreateHealthGaugeVerts();

	// キューブマップ用の頂点配列を作成
	CreateCubeMapVerts();

	// ライト用のフレームバッファ作成
	CreateLightFBO();

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

	int lightNum = 3;

	for (int i = 0; i < lightNum; i++)
	{
		Vector3 pos((rand() % (2309 - -474 + 1)) + -474,
			(rand() % (1069 - -1283 + 1)) + -1283,
			(rand() % (150 - 30 + 1)) + 30);
		mLightPos.push_back(pos);

		Vector3 color(rand() % 100 / 100.0f,
			rand() % 100 / 100.0f,
			rand() % 100 / 100.0f);
		mLightColor.push_back(color);
	}

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
	mLightSpaceMat = mDepthMapRenderer->GetLightSpaceMatrix();

	// 深度バッファへの焼きこみ処理
	BakeDepthBuffer();

	// G-Bufferへの書き込み処理
	DrawToGBuffer();

	// ライティングパス開始
	glBindFramebuffer(GL_FRAMEBUFFER, mLightFBO);
	{
		// 加算合成を有効
		glEnablei(GL_BLEND, 0);
		glBlendFuncSeparatei(0, GL_ONE, GL_ONE, GL_ONE, GL_ONE);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// G-Bufferテクスチャをシェーダーに渡す
		mGBufferRenderer->InputGBufferToShader();

		PointLightPass();

		DirectionalLightPass();

		//加算合成を無効
		glDisablei(GL_BLEND, 0);
	}
	// ライティングパス終了
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// HDRレンダリング開始
	mHDRRenderer->HDRRenderingBegin();
	{
		Shader* useShader = nullptr;

		useShader = GraphicResourceManager::FindUseShader(ShaderTag::HIGH_LIGHT);
		useShader->SetActive();

		// ライティングパスの結果をテクスチャとしてセット
		useShader->SetIntUniform("uTexture", 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mLightHDRTex);

		RenderQuad();
	}
	// HDRレンダリング終了
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

void Renderer::CreateLightFBO()
{
	unsigned int lightRBO;

	glGenFramebuffers(1, &mLightFBO);

	glBindFramebuffer(GL_FRAMEBUFFER, mLightFBO);

	// HDRテクスチャの作成
	glGenTextures(1, &mLightHDRTex);
	glBindTexture(GL_TEXTURE_2D, mLightHDRTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, mScreenWidth, mScreenHeight, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mLightHDRTex, 0);

	// OpenGLにカラーテクスチャアタッチメント0を使用することを伝える
	glDrawBuffer(GL_COLOR_ATTACHMENT0);

	// レンダーバッファを作成
	glGenRenderbuffers(1, &lightRBO);
	glBindRenderbuffer(GL_RENDERBUFFER, lightRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, mScreenWidth, mScreenHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, lightRBO);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "LightBuffer not complete!" << std::endl;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
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

void Renderer::RenderQuad()
{
	unsigned int quadVAO = 0;
	unsigned int quadVBO;

	if (quadVAO == 0)
	{
		float quadVertices[] =
		{
			//      座標      //  uv座標
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};

		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

void Renderer::BakeDepthBuffer()
{
	// デプスレンダリングパス開始
	mDepthMapRenderer->DepthRenderingBegin();
	{
		Shader* useShader = nullptr;

		// メッシュをデプスマップにレンダリング
		useShader = GraphicResourceManager::FindUseShader(ShaderTag::DEPTH_MAP);
		useShader->SetActive();
		useShader->SetMatrixUniform("lightSpaceMatrix", mLightSpaceMat);
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
		useShader->SetMatrixUniform("uLightSpaceMat", mLightSpaceMat);
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

void Renderer::DrawToGBuffer()
{
	// G-Bufferにメッシュとスケルタルメッシュを描画
	mGBufferRenderer->GBufferRenderingBegin();
	{
		Shader* useShader = nullptr;

		useShader = GraphicResourceManager::FindUseShader(ShaderTag::G_BUFFER);
		useShader->SetActive();
		useShader->SetMatrixUniform("uViewProj", mView * mProjection);

		for (auto mc : mMeshComponents)
		{
			if (mc->GetVisible())
			{
				mc->Draw(useShader);
			}
		}

		useShader = GraphicResourceManager::FindUseShader(ShaderTag::SKINNED_G_BUFFER);
		useShader->SetActive();
		useShader->SetMatrixUniform("uViewProj", mView * mProjection);
		for (auto sk : mSkeletalMeshes)
		{
			if (sk->GetVisible())
			{
				sk->Draw(useShader);
			}
		}
	}
	mGBufferRenderer->GBufferRenderingEnd();
}

void Renderer::PointLightPass()
{
	MeshComponent* sphereMesh = mHighLightMeshes[0];

	// 深度テストを無効
	glDisable(GL_DEPTH_TEST);

	// ライトボリュームの裏側だけ描画するように設定
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glFrontFace(GL_CW);


	const float constant = 1.0f;   // 定数
	const float linear = 0.7f;     // 線形
	const float quadratic = 1.8f;  //2乗項

	Shader* useShader = nullptr;

	useShader = GraphicResourceManager::FindUseShader(ShaderTag::POINT_LIGHT);

	// ポイントライトシェーダーにパラメーターをセット
	Vector3 lightSpecular = Vector3(1.0f, 1.0f, 1.0f);
	useShader->SetActive();
	useShader->SetFloatUniform("uLight.constant", constant);
	useShader->SetFloatUniform("uLight.linear", linear);
	useShader->SetFloatUniform("uLight.quadratic", quadratic);
	useShader->SetVectorUniform("uViewPos", GAMEINSTANCE.GetViewPos());
	useShader->SetVectorUniform("uLight.specular", lightSpecular);
	useShader->SetMatrixUniform("uView", mView);
	useShader->SetMatrixUniform("uProjection", mProjection);
	useShader->SetIntUniform("gPosition", 0);
	useShader->SetIntUniform("gNormal", 1);
	useShader->SetIntUniform("gAlbedoSpec", 2);
	useShader->SetFloatUniform("luminance", 10.0f);

	CalcAttenuationLightRadius(constant, linear, quadratic);

	// 個数分のライト描画
	for (unsigned int i = 0; i < mLightPos.size(); i++)
	{
		Vector3 ambient = mLightColor[i] * 0.2f;
		useShader->SetVectorUniform("uLight.position", mLightPos[i]);
		useShader->SetVectorUniform("uLight.diffuse", mLightColor[i]);
		useShader->SetVectorUniform("uLight.ambient", ambient);

		Matrix4 mat = Matrix4::CreateScale(mLightRadius[i]);
		mat = mat * Matrix4::CreateTranslation(mLightPos[i]);
		useShader->SetMatrixUniform("model", mat);

		sphereMesh->Draw(useShader, false);
	}

	// 裏側描画OFF
	glDisable(GL_CULL_FACE);
}

void Renderer::DirectionalLightPass()
{
	glDisable(GL_DEPTH_TEST);

	Shader* useShader = nullptr;

	useShader = GraphicResourceManager::FindUseShader(ShaderTag::DIRECTIONAL_LIGHT);

	// ライトカラー設定
	Vector3 ambientColor, color, specular;
	ambientColor = Vector3(0.1f, 0.1f, 0.1f);
	color = Vector3(0.3f, 0.3f, 0.3f);
	specular = Vector3(1.0f, 1.0f, 1.0f);
	float intensity = 1.0f;

	useShader->SetActive();
	useShader->SetVectorUniform("uViewPos", GAMEINSTANCE.GetViewPos());
	useShader->SetVectorUniform("uLight.direction", mDirectionalLight.mDirection);
	useShader->SetVectorUniform("uLight.ambientColor", ambientColor);
	useShader->SetVectorUniform("uLight.color", color);
	useShader->SetVectorUniform("uLight.specular", specular);
	useShader->SetFloatUniform("uLight.intensity", intensity);
	useShader->SetFloatUniform("luminance", 1.0f);
	useShader->SetMatrixUniform("uLightSpaceMatrix", mLightSpaceMat);

	// テクスチャをシェーダーにセット
	mGBufferRenderer->InputGBufferToShader();

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, mDepthMapRenderer->GetDepthTexID());
	useShader->SetIntUniform("uDepthMap", 3);



	useShader->SetIntUniform("gPosition", 0);
	useShader->SetIntUniform("gNormal", 1);
	useShader->SetIntUniform("gAlbedoSpec", 2);


	// スクリーンいっぱいの四角形を描画
	RenderQuad();
}

void Renderer::CalcAttenuationLightRadius(const float constant, const float linear, const float quadratic)
{
	// 解の公式より減衰半径を計算
	for (unsigned int i = 0; i < mLightColor.size(); i++)
	{
		Vector3 color = mLightColor[i];
		float max = std::fmax(std::fmax(color.x, color.y), color.z);
		float radius = (-linear + std::sqrtf((linear * linear) - 4.0f *
			quadratic * (constant - (256.0f / 4.0f) * max))) / (2.0f * quadratic);

		mLightRadius.push_back(radius);
	}
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