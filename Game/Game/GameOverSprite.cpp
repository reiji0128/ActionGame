#include "GameOverSprite.h"
#include "Game.h"
#include "GraphicResourceManager.h"
#include "Renderer.h"
#include "Texture.h"
#include "Shader.h"

GameOverSprite::GameOverSprite(const Vector2& pos, const Vector2& scale, const float& alpha)
	:UIScreen(pos,scale,alpha)
	, mFadeInSpeed(0.0f)
	, mNormalFadeSpeed(0.1f)
	, IsAdition(true)
{
	// テクスチャの生成
	mTexture = GraphicResourceManager::LoadTexture("Assets/UI/GameOver/GameOver.png");

	mFadeInSpeed = mNormalFadeSpeed;
}

GameOverSprite::~GameOverSprite()
{
}

void GameOverSprite::Update(float deltaTime)
{
	FadeIn(deltaTime);
}

void GameOverSprite::Draw()
{
	// 頂点配列をアクティブに変更
	RENDERER->ChangeActievSpriteVertex();

	Shader* useShader = nullptr;
	useShader = GraphicResourceManager::FindUseShader(ShaderTag::SPRITE);

	// テクスチャの幅・高さのスケール行列の計算
	Matrix4 scaleMat = Matrix4::CreateScale(static_cast<float>(mTexture->GetWidth()) * mScale.x,
		static_cast<float>(mTexture->GetHeight()) * mScale.y,
		1.0f);

	// スクリーン位置の平行移動
	Matrix4 transMat = Matrix4::CreateTranslation(Vector3(mOffset.x - (mScreenWidth * 0.5f),
		(mScreenHeight * 0.5f) - mOffset.y,
		0.0f));

	// ワールド変換行列の計算
	Matrix4 world = scaleMat * transMat;

	// シェーダーに必要な値を渡す
	useShader->SetActive();
	useShader->SetMatrixUniform("uWorldTransform", world);
	useShader->SetMatrixUniform("uViewProj", RENDERER->GetSimpleViewProjMatrix());
	useShader->SetFloatUniform("uAlpha", mAlpha);
	// テクスチャのセット
	mTexture->SetActive();


	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

void GameOverSprite::FadeIn(float deltaTime)
{
	if (IsAdition)
	{
		mAlpha += mFadeInSpeed * deltaTime;
	}

	if (mAlpha >= 1.0f)
	{
		IsAdition = false;
	}
}
