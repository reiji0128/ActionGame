#include "HitPointGaugeFrame.h"
#include "Game.h"
#include "GraphicResourceManager.h"
#include "Renderer.h"
#include "Texture.h"
#include "Shader.h"

HitPointGaugeFrame::HitPointGaugeFrame(const Vector2& offset, const Vector2& scale, const float& alpha,const char* textureFilePass)
	:UIScreen(offset,scale,alpha)
{
	// �e�N�X�`���̐���
	mTexture = GraphicResourceManager::LoadTexture(textureFilePass);
}

HitPointGaugeFrame::~HitPointGaugeFrame()
{
}

void HitPointGaugeFrame::Draw()
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
