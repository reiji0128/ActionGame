#pragma once

class Shader;

class GBuffer
{
public:
	GBuffer();

	~GBuffer();

	/// <summary>
	/// G-Buffer�����_�����O�̊J�n
	/// </summary>
	void GBufferRenderingBegin();


	/// <summary>
	/// G-Buffer�����_�����O�̏I��
	/// </summary>
	void GBufferRenderingEnd();

	/// <summary>
	/// G-Buffer�̍쐬
	/// </summary>
	void CreateGBuffer();

	/// <summary>
	/// G-Buffer���V�F�[�_�[�ɓ��͂ɓn��
	/// </summary>
	void InputGBufferToShader();

// �Q�b�^�[ //
	unsigned int GetGBufferID() const { return mGBuffer; }

	int GetDepthTextureID() const { return mDepth; }

private:
	int mScreenWidth;

	int mScreenHeight;

	// G-Buffer��ID
	unsigned int mGBuffer;

	// ���W�e�N�X�`����ID
	unsigned int mGPosition;

	// �@���e�N�X�`����ID
	unsigned int mGNormal;

	// �A���x�h�ƃX�y�L�����e�N�X�`����ID
	unsigned int mGAlbedoSpec;

	// �[�x�e�N�X�`����ID
	unsigned int mDepth;
};