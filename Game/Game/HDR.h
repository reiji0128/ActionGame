#pragma once
#include <SDL.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include "Math.h"

class HDR
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	HDR();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~HDR();
	
	/// <summary>
	/// HDR�����_�����O�̊J�n����
	/// </summary>
	void HDRRenderingBegin();

	/// <summary>
	/// HDR�����_�����O�̏I������
	/// </summary>
	void HDRRenderingEnd();

	/// <summary>
	/// ���������_�t���[���o�b�t�@�̍쐬
	/// </summary>
	void CreateHDRBuffer();

	/// <summary>
	/// �k���o�b�t�@�̍쐬(�_�E���T���v�����O&�ڂ����p�̂��߂̃t���[���o�b�t�@)
	/// </summary>
	void CreateScaleDownBuffer();

	/// <summary>
	/// �k���o�b�t�@�p�X
	/// </summary>
	void ScaleDownBufferPath();

	/// <summary>
	/// �K�E�X�ڂ����̏d�݌W���v�Z
	/// </summary>
	/// <param name="pos">�t���O�����g�̈ʒu</param>
	/// <param name="rho">�΍�(�傫������Ƃ�܂��Ȃ��炩�ɂȂ�)</param>
	/// <returns>�K�E�X�ڂ����̏d�݌W��</returns>
	float GaussianDistribution(const Vector2& pos, float rho);

	/// <summary>
	/// �K�E�X�ڂ����Ȑ��֐�
	/// </summary>
	void CalcGaussBlurParam(int w, int h, Vector2 dir,float deviation, Vector3* offset);

	/// <summary>
	/// �l�p�`�|���S���̍쐬
	/// </summary>
	void CreateQuadVAO();

	/// <summary>
	/// �l�p�`�|���S���̕`��
	/// </summary>
	void RenderQuad();

	/// <summary>
	/// HDR�e�N�X�`����Bloom�e�N�X�`���̃u�����h
	/// </summary>
	void HDRBloomBlend();

private:
	unsigned int mHdrFBO;
	unsigned int mRbo;
	unsigned int mFloatColorTexture;

	unsigned int mColorBuffers[2];

	// �k���o�b�t�@���x����
	unsigned int mMaxLevelNum;

	unsigned int mBufferWidth;
	unsigned int mBufferHeght;

	// ���_�z��I�u�W�F�N�gID
	unsigned int mVertexArray;
	// ���_�o�b�t�@ID
	unsigned int mVertexBuffer;
	// �C���f�b�N�XID
	unsigned int mIndexBuffer;

	// �u���[�p��FBO
	std::vector<unsigned int> mBlurFBO;
	// �u���[���ʂ̃e�N�X�`��ID
	std::vector<unsigned int> mBlurBufferTex;
};