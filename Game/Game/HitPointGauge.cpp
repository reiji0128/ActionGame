#include "HitPointGauge.h"
#include "Game.h"
#include "Renderer.h"
#include "Shader.h"
#include "Texture.h"
#include "GameObject.h"
#include "GraphicResourceManager.h"
#include "IHitPointGetter.h"

HitPointGauge::HitPointGauge(const Vector2& offset, const Vector2& scale, float alpha, const char* textureFilePass, IHitPointGetter* playerParameterGetter)
	:UIScreen(offset, scale, alpha)
	,mNowHP(0)
{
	// �e�N�X�`���̐���
	mTexture = GraphicResourceManager::LoadTexture(textureFilePass);

	// �C���^�[�t�F�C�X����ő�̗͂��擾
	mHitPointGetter = playerParameterGetter;
	mMaxHP = mHitPointGetter->GetMaxHP();
}

HitPointGauge::~HitPointGauge()
{
}

void HitPointGauge::Draw()
{
	// ���_�z����A�N�e�B�u�ɕύX
	RENDERER->ChangeActiveHPGaugeVertex();
	
	// �̗̓Q�[�W�̃X�P�[���v�Z
	CalcGaugeScale();

	Shader* useShader = nullptr;
	useShader = GraphicResourceManager::FindUseShader(ShaderTag::TILEMAP);

	// �e�N�X�`���̕��E�����̃X�P�[���s��̌v�Z
	Matrix4 scaleMat = Matrix4::CreateScale(static_cast<float>(mTexture->GetWidth())  * resultScale,
		                                    static_cast<float>(mTexture->GetHeight()) * mScale.y,
                                            1.0f);

	// �X�N���[���ʒu�̕��s�ړ�
	Matrix4 transMat = Matrix4::CreateTranslation(Vector3(mOffset.x - (mScreenWidth * 0.5f),
		                                                 (mScreenHeight * 0.5f) - mOffset.y,
		                                                  0.0f));

	// ���[���h�ϊ��s��̌v�Z
	Matrix4 world = scaleMat * transMat;

	Vector2 tileSplitNum(static_cast<float>(1), static_cast<float>(1));

	// �V�F�[�_�[�ɕK�v�Ȓl��n��
	useShader->SetActive();
	useShader->SetMatrixUniform("uWorldTransform", world);
	useShader->SetIntUniform("uTileIndex", 0);
	useShader->SetVector2Uniform("uTileSetSplitNum", tileSplitNum);
	useShader->SetFloatUniform("uAlpha", mAlpha);
	useShader->SetMatrixUniform("uViewProj", RENDERER->GetSimpleViewProjMatrix());

	// �e�N�X�`���̃Z�b�g
	mTexture->SetActive();

	// �l�p�`�`��
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

void HitPointGauge::CalcGaugeScale()
{
	// ���݂̗̑͂��擾
	mNowHP = mHitPointGetter->GetHP();

	//�̗͔䗦�v�Z
	float hpRate = mNowHP / mMaxHP;
	resultScale = hpRate / 2 * mScale.x;
}
