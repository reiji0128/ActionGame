#include "ImageComponent.h"
#include "Game.h"
#include "Renderer.h"
#include "Texture.h"
#include "Shader.h"
#include "UIScreen.h"

ImageComponent::ImageComponent(UIScreen* owner, Texture* texture, int drawOrder)
	:UIComponent(owner,UITag::Image,drawOrder)
{
	// �e�N�X�`���̃Z�b�g
	mTexture = texture;

	// ���W�E�X�P�[���E���ߓx�̃Z�b�g
	mOffset = owner->GetOffset();
	mScale = owner->GetScale();
	mAlpha = owner->GetAlpha();

	// �����_�[�N���X�ɒǉ�
	RENDERER->AddUI(this);
}

ImageComponent::~ImageComponent()
{
	// �����_�[�N���X����폜
	RENDERER->RemoveUI(this);
}

void ImageComponent::Draw(Shader* shader)
{
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

	// �V�F�[�_�[�ɕK�v�Ȓl���Z�b�g
	shader->SetActive();
	shader->SetMatrixUniform("uWorldTransform", world);
	shader->SetMatrixUniform("uViewProj", RENDERER->GetSimpleViewProjMatrix());
	shader->SetFloatUniform("uAlpha", mAlpha);

	// �e�N�X�`�����A�N�e�B�u�ɃZ�b�g
	mTexture->SetActive();

	// �`��
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}
