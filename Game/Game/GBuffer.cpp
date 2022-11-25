#include "GBuffer.h"
#include "Shader.h"
#include "Game.h"
#include "Renderer.h"


GBuffer::GBuffer()
{
	mScreenWidth  = RENDERER->GetScreenWidth();
	mScreenHeight = RENDERER->GetScreenHeight();
}

GBuffer::~GBuffer()
{
}

void GBuffer::GBufferRenderingBegin()
{
	// 描画先をGbufferにセット
	glBindFramebuffer(GL_FRAMEBUFFER, mGBuffer);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDepthMask(GL_TRUE);
}

void GBuffer::GBufferRenderingEnd()
{
	// 描画先をスクリーンに戻す
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GBuffer::CreateGBuffer()
{
	unsigned int gRenderBuffer;

	// G-Bufferの作成
	glGenFramebuffers(1, &mGBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, mGBuffer);
	{
		// 位置用カラーバッファの作成
		glGenTextures(1, &mGPosition);
		glBindTexture(GL_TEXTURE_2D, mGPosition);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, mScreenWidth, mScreenHeight, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mGPosition, 0);

		// 法線用カラーバッファの作成
		glGenTextures(1, &mGNormal);
		glBindTexture(GL_TEXTURE_2D, mGNormal);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, mScreenWidth, mScreenHeight, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, mGNormal, 0);

		// アルベド(RGB) & スペキュラー(A)用カラーバッファの作成
		glGenTextures(1, &mGAlbedoSpec);
		glBindTexture(GL_TEXTURE_2D, mGAlbedoSpec);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mScreenWidth, mScreenHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, mGAlbedoSpec, 0);

		// 深度バッファの作成
		glGenTextures(1, &mDepth);
		glBindTexture(GL_TEXTURE_2D, mDepth);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mScreenWidth, mScreenHeight, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, mDepth, 0);

		// レンダリングに使用するカラーバッファのアタッチメント
		unsigned int attachments[4] =
		{
			GL_COLOR_ATTACHMENT0,
			GL_COLOR_ATTACHMENT1,
			GL_COLOR_ATTACHMENT2,
			GL_COLOR_ATTACHMENT3
		};

		glDrawBuffers(4, attachments);

		// レンダーバッファーの作成
		glGenRenderbuffers(1, &gRenderBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, gRenderBuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, mScreenWidth, mScreenHeight);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, gRenderBuffer);

		// フレームバッファーにエラーがないか確認
		GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

		if (Status != GL_FRAMEBUFFER_COMPLETE) 
		{
			printf("FB error, status: 0x%x\n", Status);
		}
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GBuffer::InputGBufferToShader()
{
	// テクスチャスロット0番に座標テクスチャをバインド
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mGPosition);

	// テクスチャスロット1番に法線テクスチャをバインド
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, mGNormal);

	// テクスチャスロット2番にアルベドとスペキュラテクスチャをバインド
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, mGAlbedoSpec);
}
