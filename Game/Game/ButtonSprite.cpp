#include "ButtonSprite.h"
#include "Game.h"
#include "GraphicResourceManager.h"
#include "Renderer.h"
#include "Texture.h"
#include "Shader.h"
#include "Input.h"

ButtonSprite::ButtonSprite(const Vector2& offset, const Vector2& scale, const float& alpha, const char* texFilePass)
	:UIScreen(offset,scale,alpha)
	,mFadeInSpeed(0.0f)
	,mNormalFadeSpeed(0.4)
	,mDecisionFadeSpeed(0.8)
	,IsAdition(false)
{
	// �e�N�X�`���̐���
	mTexture = GraphicResourceManager::LoadTexture(texFilePass);

	mFadeInSpeed = mNormalFadeSpeed;
}

ButtonSprite::~ButtonSprite()
{
}

void ButtonSprite::Update(float deltaTime)
{
	FadeIn(deltaTime);

	FadeOut(deltaTime);

	if (INPUT_INSTANCE.IsKeyPushdown(KEY_START))
	{
		mFadeInSpeed = mDecisionFadeSpeed;
	}
}

void ButtonSprite::Draw()
{
	// ���_�z����A�N�e�B�u�ɕύX
	RENDERER->ChangeActievSpriteVertex();

	Shader* useShader = nullptr;
	useShader = GraphicResourceManager::FindUseShader(ShaderTag::SPRITE);

	// �e�N�X�`���̕��E�����̃X�P�[���s��̌v�Z
	Matrix4 scaleMat = Matrix4::CreateScale(static_cast<float>(mTexture->GetWidth()) * mScale.x,
		static_cast<float>(mTexture->GetHeight()) * mScale.y,
		1.0f);

	// �X�N���[���ʒu�̕��s�ړ�
	Matrix4 transMat = Matrix4::CreateTranslation(Vector3(mOffset.x - (mScreenWidth * 0.5f),
		(mScreenHeight * 0.5f) - mOffset.y,
		0.0f));

	// ���[���h�ϊ��s��̌v�Z
	Matrix4 world = scaleMat * transMat;

	// �V�F�[�_�[�ɕK�v�Ȓl��n��
	useShader->SetActive();
	useShader->SetMatrixUniform("uWorldTransform", world);
	useShader->SetMatrixUniform("uViewProj", RENDERER->GetSimpleViewProjMatrix());
	useShader->SetFloatUniform("uAlpha", mAlpha);
	// �e�N�X�`���̃Z�b�g
	mTexture->SetActive();


	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

void ButtonSprite::FadeIn(float deltaTime)
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

void ButtonSprite::FadeOut(float deltaTime)
{
	if (!IsAdition)
	{
		mAlpha -= mFadeInSpeed * deltaTime;
	}

	if (mAlpha <= 0.0f)
	{
		IsAdition = true;
	}
}
