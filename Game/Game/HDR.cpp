#include "HDR.h"
#include "Shader.h"
#include <iostream>
#include "Game.h"
#include "Renderer.h"
#include "GraphicResourceManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
HDR::HDR()
{
	mBufferWidth = RENDERER->GetScreenWidth();
	mBufferHeght = RENDERER->GetScreenHeight();

	// �������_�t���[���o�b�t�@�̍쐬
	CreateHDRBuffer();
	// �k���o�b�t�@�̍쐬
	CreateScaleDownBuffer();
	// �l�p�`�|���S���̍쐬
	CreateQuadVAO();
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
HDR::~HDR()
{
}

/// <summary>
/// HDR�����_�����O�̊J�n����
/// </summary>
void HDR::HDRRenderingBegin()
{
	glBindFramebuffer(GL_FRAMEBUFFER, mHdrFBO);
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

/// <summary>
/// HDR�����_�����O�̏I������
/// </summary>
void HDR::HDRRenderingEnd()
{
	// �`��Ώۂ��X�N���[���֖߂�
	glBindFramebuffer(GL_FRAMEBUFFER,0);
}

/// <summary>
/// ���������_�t���[���o�b�t�@�ƍ��P�x�o�b�t�@�̍쐬
/// </summary>
void HDR::CreateHDRBuffer()
{
	glGenFramebuffers(1, &mHdrFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, mHdrFBO);

	glGenTextures(2, mColorBuffers);

	for (unsigned int i = 0; i < 2; i++)
	{
		glBindTexture(GL_TEXTURE_2D, mColorBuffers[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, mBufferWidth, mBufferHeght, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		// �t���[���o�b�t�@�ɃJ���[�e�N�X�`���Ƃ���colorBuffer���A�^�b�`
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, mColorBuffers[i], 0);
	}


	// �����_�[�o�b�t�@�[�̍쐬
	glGenRenderbuffers(1, &mRbo);
	glBindRenderbuffer(GL_RENDERBUFFER, mRbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, mBufferWidth, mBufferHeght);

	// FBO�Ƀ����_�[�o�b�t�@�[���A�^�b�`����
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mRbo);

	unsigned int attachments[2] =
	{
		GL_COLOR_ATTACHMENT0,
		GL_COLOR_ATTACHMENT1
	};

	glDrawBuffers(2, attachments);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete" << std::endl;
	}
	// �f�t�H���g�ɖ߂�
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

/// <summary>
/// �k���o�b�t�@�̍쐬(�_�E���T���v�����O&�ڂ����p�̂��߂̃t���[���o�b�t�@)
/// </summary>
void HDR::CreateScaleDownBuffer()
{
	mMaxLevelNum = 5;         // �k���o�b�t�@���x����(1/2, 1/4, 1/8, 1/16, 1/32 5�̒i�K)

	int w = mBufferWidth;
	int h = mBufferHeght;

	float borderColor[4] = { 0.0f };

	// FBO�ƃe�N�X�`��ID�m�ۗp
	mBlurFBO.resize(mMaxLevelNum * 2);
	mBlurBufferTex.resize(mMaxLevelNum * 2);

	// �������E�c�����u���[�쐬
	// mmMaxLevelNum��2�{���̖����쐬
	for (unsigned int i = 0; i < mMaxLevelNum; i++)
	{
		// �k���o�b�t�@�̕��E�����̌v�Z
		w /= 2;
		h /= 2;

		for (int j = 0; j < 2; j++)
		{
			glGenFramebuffers(1, &mBlurFBO[i * 2 + j]);
			glGenTextures(1, &mBlurBufferTex[i * 2 + j]);
			{
				glBindFramebuffer(GL_FRAMEBUFFER, mBlurFBO[i * 2 + j]);
				glBindTexture(GL_TEXTURE_2D, mBlurBufferTex[i * 2 + j]);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, w, h, 0, GL_RGBA, GL_FLOAT, NULL);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mBlurBufferTex[i * 2 + j], 0);
			}
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
	}
}

/// <summary>
/// �k���o�b�t�@�p�X
/// </summary>
void HDR::ScaleDownBufferPath()
{
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	unsigned int renderSource = mColorBuffers[1];     // �_�E���T���v�����O������Ώۂ��P�x�o�b�t�@�Ɏw��

	// �k���o�b�t�@1,3,5,7,9�ɃR�s�[
	int reduceX = mBufferWidth;
	int reduceY = mBufferHeght;

	// �_�E���T���v�����O 1,3,5 ...�Ɗ�ԂɃ_�E���T���v�����O���ʂ��o��
	for (int i = 0; i < mMaxLevelNum; i++)
	{
		reduceX /= 2;
		reduceY /= 2;

		// �`��FBO�Ɋ�Ԃ��w��
		glBindFramebuffer(GL_FRAMEBUFFER, mBlurFBO[i * 2 + 1]);
		{
			Shader* useShader = nullptr;
			useShader = GraphicResourceManager::FindUseShader(ShaderTag::DOWNSAMPLING);
			glViewport(0, 0, reduceX, reduceY);

			// �J���[�o�b�t�@�̃N���A
			glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, renderSource);
			useShader->SetActive();
			useShader->SetIntUniform("uScreenTexture", 0);

			// �X�N���[���S�̂ɕ`��
			glBindVertexArray(mVertexArray);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
		}
		renderSource = mBlurBufferTex[i * 2 + 1];     // �`��I�������e�N�X�`��������Ƀ_�E���T���v�����O�̑Ώۂ�
	}

	// �K�E�X�ڂ������� //
	const unsigned int sampleCount = 15;
	Vector3 offset[sampleCount];

	reduceX = mBufferWidth;
	reduceY = mBufferHeght;
	float deviation = 5.0f;
	renderSource = mBlurBufferTex[1];

	// �K�E�X���x������
	for (int i = 0; i < mMaxLevelNum; i++)
	{
		reduceX /= 2;
		reduceY /= 2;

		// horizontal  0 : ����  1 : ��������  �ɃK�E�X�ڂ�����������
		for (int horizontal = 0; horizontal < 2; horizontal++)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, mBlurFBO[i * 2 + horizontal]);
			{
				Shader* useShader = nullptr;
				useShader = GraphicResourceManager::FindUseShader(ShaderTag::GAUSS_BLUR);

				glViewport(0, 0, reduceX, reduceY);
				Vector2 dir;
				if (horizontal)
				{
					dir = Vector2(0, 1);
				}
				else
				{
					dir = Vector2(1, 0);
				}
				CalcGaussBlurParam(reduceX, reduceY, dir, deviation, offset);

				// �J���[�o�b�t�@�[�̃N���A
				glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
				glClear(GL_COLOR_BUFFER_BIT);

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, renderSource);
				useShader->SetActive();
				useShader->SetIntUniform("uBlursource", 0);
				useShader->SetIntUniform("uParam.SampleCount", 15);

				// �K�E�X�V�F�[�_�[��Offset���Z�b�g
				for (int i = 0; i < sampleCount; i++)
				{
					std::string s = "uParam.Offset[" + std::to_string(i) + "]";
					useShader->SetVectorUniform(s.c_str(), offset[i]);
				}

				// �X�N���[���S�̂ɕ`��
				glBindVertexArray(mVertexArray);
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

				renderSource = mBlurBufferTex[i * 2 + horizontal];         // �`��I�������e�N�X�`��������Ƀ_�E���T���v�����O�̑Ώۂ�
			}
		}
		// �񐔂��Ƃɕ΍����グ��
		deviation *= deviation;
	}
}

/// <summary>
/// �K�E�X�ڂ����̏d�݌W���v�Z
/// </summary>
/// <param name="pos">�t���O�����g�̈ʒu</param>
/// <param name="rho">�΍�(�傫������Ƃ�܂��Ȃ��炩�ɂȂ�)</param>
/// <returns>�K�E�X�ڂ����̏d�݌W��</returns>
float HDR::GaussianDistribution(const Vector2& pos, float rho)
{
	return exp(-(pos.x * pos.x + pos.y * pos.y) / (2.0f * rho * rho));
}

/// <summary>
/// �K�E�X�ڂ����Ȑ��֐�
/// </summary>
void HDR::CalcGaussBlurParam(int w, int h, Vector2 dir, float deviation, Vector3* offset)
{
	auto tu = 1.0f / float(w);
	auto tv = 1.0f / float(h);

	offset[0].z = GaussianDistribution(Vector2(0.0f, 0.0f), deviation);
	auto total_weight = offset[0].z;

	offset[0].x = 0.0f;
	offset[0].y = 0.0f;

	for (auto i = 1; i < 8; ++i)
	{
		int nextpos = (i - 1) * 2 + 1;
		offset[i].x = dir.x * tu * nextpos;
		offset[i].y = dir.y * tv * nextpos;
		offset[i].z = GaussianDistribution(dir * float(nextpos), deviation);
		total_weight += offset[i].z * 2.0f;
	}

	for (auto i = 0; i < 8; ++i)
	{
		offset[i].z /= total_weight;
	}
	for (auto i = 8; i < 15; ++i)
	{
		offset[i].x = -offset[i - 7].x;
		offset[i].y = -offset[i - 7].y;
		offset[i].z = offset[i - 7].z;
	}
}


/// <summary>
/// �l�p�`�|���S���̍쐬
/// </summary>
void HDR::CreateQuadVAO()
{
	float vertices[] =
	{
		//   �ʒu      |    uv���W
		-1.0f,  1.0f,     0.0f, 1.0f,       // ����  0
		-1.0f, -1.0f,     0.0f, 0.0f,       // ����  1
		 1.0f,  1.0f,     1.0f, 1.0f,       // �E��  2
		 1.0f, -1.0f,     1.0f, 0.0f,       // �E��  3
	};

	unsigned int indices[] =
	{
		0,1,2,
		2,3,1
	};

	// ���_�z��̍쐬
	glGenVertexArrays(1, &mVertexArray);
	glBindVertexArray(mVertexArray);

	// ���_�o�b�t�@�̍쐬
	glGenBuffers(1, &mVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexArray);
	glBufferData(GL_ARRAY_BUFFER, 4 * 4 * sizeof(float), vertices, GL_STATIC_DRAW);

	// �C���f�b�N�X�o�b�t�@�̍쐬
	glGenBuffers(1, &mIndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

	// Attribute 0 �ʒu
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, 0);

	// Attribute 1 uv���W
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, reinterpret_cast<void*>(sizeof(float) * 2));
}

/// <summary>
/// �l�p�`�|���S���̕`��
/// </summary>
void HDR::RenderQuad()
{
	Shader* useShader = nullptr;
	useShader = GraphicResourceManager::FindUseShader(ShaderTag::SCREEN_BUFFER);

	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

	useShader->SetActive();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mColorBuffers[0]);
	useShader->SetIntUniform("uHDRBuffer", 0);
	useShader->SetFloatUniform("exposure", 0.3);
	glBindVertexArray(mVertexArray);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glDisable(GL_BLEND);
}

/// <summary>
/// HDR�e�N�X�`����Bloom�e�N�X�`���̃u�����h
/// </summary>
void HDR::HDRBloomBlend()
{
	// �����_�[�^�[�Q�b�g�����Ƃɖ߂�
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDrawBuffer(GL_FRONT);

	glViewport(0, 0, mBufferWidth, mBufferHeght);

	Shader* useShader = nullptr;
	useShader = GraphicResourceManager::FindUseShader(ShaderTag::HDR_BLOOM_BLEND);

	// TEXTURE0��HDR�e�N�X�`�����o�C���h
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mColorBuffers[0]);

	// TEXTURE1,2,3,4,5��Bloom�e�N�X�`�����o�C���h
	for (int i = 0; i < mMaxLevelNum; i++)
	{
		glActiveTexture(GL_TEXTURE1 + i);
		glBindTexture(GL_TEXTURE_2D, mBlurBufferTex[i * 2 + 1]);
	}

	useShader->SetActive();
	useShader->SetIntUniform("uScene", 0);
	useShader->SetIntUniform("uBloom1", 1);
	useShader->SetIntUniform("uBloom2", 2);
	useShader->SetIntUniform("uBloom3", 3);
	useShader->SetIntUniform("uBloom4", 4);
	useShader->SetIntUniform("uBloom5", 5);

	const float exposure = 0.4;

	useShader->SetFloatUniform("uExposure", exposure);

	glBindVertexArray(mVertexArray);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}
