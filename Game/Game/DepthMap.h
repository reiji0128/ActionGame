#pragma once
#include <SDL.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Math.h"
#include "Shader.h"

class DepthMap
{
public:
	DepthMap();
	~DepthMap();
	void DepthRenderingBegin();
	void DepthRenderingEnd();
	void CreateShadowMap();
	void CalcLightSpaceMatrix(const Vector3& centerWorldPos,const Vector3& lightDir,const Vector3& upVec,const float lightDistance);

	// �Q�b�^�[ //
	unsigned int GetDepthTexID() { return mDepthMap; }

	Vector3& GetLightDir() { return mLightDir; }

	Vector3& GetLightPod() { return mLightPos; }

	Matrix4& GetLightSpaceMatrix() { return mLightSpaceMatrix; }
private:
	// �f�v�X�}�b�v�t���[���o�b�t�@�I�u�W�F�N�g
	unsigned int mDepthMapFBO;

	// �f�v�X�e�N�X�`��
	unsigned int mDepthMap;

	const unsigned int SHADOW_WIDTH  = 4096;
	const unsigned int SHADOW_HEIGHT = 4096;

	//�f�v�X�}�b�v�����_�����O�̎B�e���_
	Vector3 mLightPos;

	// �����x�N�g������
	Vector3 mLightDir;

	// ���C�g���
	Matrix4 mLightSpaceMatrix;
};