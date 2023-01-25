#include "GaugeComponent.h"
#include "Game.h"
#include "Renderer.h"
#include "Texture.h"
#include "Shader.h"
#include "UIScreen.h"

GaugeComponent::GaugeComponent(UIScreen* owner, Texture* texture,float maxValue,int drawOrder)
	:UIComponent(owner,UITag::Gauge,drawOrder)
	,mNowValue(maxValue)
	,mMaxValue(maxValue)
{
	// �e�N�X�`���̃Z�b�g
	mTexture = texture;

	// ���W�E�X�P�[���E���ߓx�̃Z�b�g
	mOffset = owner->GetOffset();
	mScale = owner->GetScale();
	mAlpha = owner->GetAlpha();

	// �Q�[�W�̃X�P�[���̌v�Z
	CalcGaugeScale(mNowValue);

	// �����_�[�N���X�ɒǉ�
	RENDERER->AddUI(this);
}

GaugeComponent::~GaugeComponent()
{
	// �����_�[�N���X����폜
	RENDERER->RemoveUI(this);
}

void GaugeComponent::Draw(Shader* shader)
{
	// �e�N�X�`���̕��E�����̃X�P�[���s��̌v�Z
	Matrix4 scaleMat = Matrix4::CreateScale(static_cast<float>(mTexture->GetWidth()) * mResultScale,
		                                    static_cast<float>(mTexture->GetHeight()) * mScale.y,
		                                    1.0f);

	// �X�N���[���ʒu�̕��s�ړ�
	Matrix4 transMat = Matrix4::CreateTranslation(Vector3(mOffset.x - (mScreenWidth * 0.5f),
		                                          (mScreenHeight * 0.5f) - mOffset.y,
		                                          0.0f));

	// ���[���h�ϊ��s��̌v�Z
	Matrix4 world = scaleMat * transMat;

	Vector2 tileSplitNum(static_cast<float>(1), static_cast<float>(1));

	// �V�F�[�_�[�ɕK�v�Ȓl���Z�b�g
	shader->SetActive();
	shader->SetMatrixUniform("uWorldTransform", world);
	shader->SetIntUniform("uTileIndex", 0);
	shader->SetVector2Uniform("uTileSetSplitNum", tileSplitNum);
	shader->SetFloatUniform("uAlpha", mAlpha);
	shader->SetMatrixUniform("uViewProj", RENDERER->GetSimpleViewProjMatrix());

	// �e�N�X�`�����A�N�e�B�u�ɃZ�b�g
	mTexture->SetActive();

	// �l�p�`�`��
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

void GaugeComponent::CalcGaugeScale(const float& nowValue)
{
	mNowValue = nowValue;

	//�̗͂̔䗦�v�Z
	float hpRate = mNowValue / mMaxValue;
	mResultScale = hpRate / 2 * mScale.x;
}
